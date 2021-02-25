#include "pdp.h"
#include "log.h"
#include "ape_error.h"
#include "ape_interface.h"
#include "common.h"
#include "ape_engine_internal.h"
#include "ape_config.h"
#include "ape_debug.h"
#define MAX_SPLIT_NUM	64
#define ARRAY_SIZE(a)	(sizeof(a) / sizeof((a[0])))


static const ScU8 map_ram[] = {
	FIELD_ENUM(PDP_RDMA_D_SRC_RAM_CFG_0, SRC_RAM_TYPE, MC),
	FIELD_ENUM(PDP_RDMA_D_SRC_RAM_CFG_0, SRC_RAM_TYPE, CV),
};

static const ScU8 map_pool[] = {
	FIELD_ENUM(PDP_D_OPERATION_MODE_CFG_0,
			POOLING_METHOD, POOLING_METHOD_AVERAGE),
	FIELD_ENUM(PDP_D_OPERATION_MODE_CFG_0,
			POOLING_METHOD, POOLING_METHOD_MAX),
	FIELD_ENUM(PDP_D_OPERATION_MODE_CFG_0,
			POOLING_METHOD, POOLING_METHOD_MIN),
};

static const ScU8 map_precision[] = {
	FIELD_ENUM(PDP_D_DATA_FORMAT_0, INPUT_DATA, INT8),
	FIELD_ENUM(PDP_D_DATA_FORMAT_0, INPUT_DATA, INT16),
	FIELD_ENUM(PDP_D_DATA_FORMAT_0, INPUT_DATA, FP16),
};

static const ScU8 map_pool_kernel[] = {
	FIELD_ENUM(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_WIDTH, KERNEL_WIDTH_1),
	FIELD_ENUM(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_WIDTH, KERNEL_WIDTH_2),
	FIELD_ENUM(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_WIDTH, KERNEL_WIDTH_3),
	FIELD_ENUM(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_WIDTH, KERNEL_WIDTH_4),
	FIELD_ENUM(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_WIDTH, KERNEL_WIDTH_5),
	FIELD_ENUM(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_WIDTH, KERNEL_WIDTH_6),
	FIELD_ENUM(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_WIDTH, KERNEL_WIDTH_7),
	FIELD_ENUM(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_WIDTH, KERNEL_WIDTH_8),
};

static const ScU32 recip_kernel_size[2][8] = {
	/*
	 * INT8/16
	 * 1      1/2     1/3     1/4     1/5     1/6     1/7     1/8
	 */
	{0x10000, 0x8000, 0x5555, 0x4000, 0x3333, 0x2aaa, 0x2492, 0x2000},
	{0x7c00, 0x7800, 0x7555,  0x7400, 0x7266, 0x7155, 0x7092, 0x7000},
};


static int
validate_strides(ScU8 stride_x, ScU8 stride_y)
{
	ScS32 ret = 0;

	if (stride_x < 1 || stride_y < 1 || stride_x > 8 || stride_y > 8) {
		LOG_DEBUG("Invalid Stride (x[%d], y[%d])\n", stride_x, stride_y);
		ret = not_initialized;
	}

	return;
}

static int
vaildate_pdp_configs(struct ape_cmd *pdp_data)
{
	ScS32 ret = 0;
	struct pdp_op_desc *pdp_op;
	struct pdp_surface_desc *pdp_surface;

	pdp_op = (struct pdp_op_desc *)pdp_data->op_addr;
	pdp_surface = (struct pdp_surface_desc *)pdp_data->surface_addr;

	if (pdp_surface->dst_data.type == APE_MEM_HW) {
		LOG_DEBUG("Destination buffer for PDP has to be either MC or CV");
		ret = not_initialized ;
		goto exit;
	}

	ret = validate_data_cube(pdp_surface->src_data, pdp_surface->dst_data,
								APE_MEM_HW);
	if (ret)
		goto exit;

	ret = validate_precision(pdp_op->precision, ARRAY_SIZE(map_precision));
	if (ret)
		goto exit;

	ret = validate_strides(pdp_op->stride_x, pdp_op->stride_y);
	if (ret)
		goto exit;

	if (pdp_op->split_num > MAX_SPLIT_NUM) {
		LOG_DEBUG("Invalid split_num: %u\n", pdp_op->split_num);
		ret = not_initialized ;
		goto exit;
	}

	if (pdp_op->pool_width >= ARRAY_SIZE(map_pool_kernel)) {
		LOG_DEBUG("Invalid pool_width: %u\n", pdp_op->pool_width);
		ret = not_initialized ;
		goto exit;
	}

	if (pdp_op->pool_height >= ARRAY_SIZE(map_pool_kernel)) {
		LOG_DEBUG("Invalid pool_height: %u\n", pdp_op->pool_height);
		ret = not_initialized ;
		goto exit;
	}

	if (pdp_op->pool_mode >= ARRAY_SIZE(map_pool)) {
		LOG_DEBUG("Invalid pool_mode: %u\n", pdp_op->pool_mode);
		ret = not_initialized ;
		goto exit;
	}

exit:
	return;
}

void pdp_enable(void * data) {
    LOG_DEBUG("enter\n");
    ScU32 reg;
	struct ape_cmd * pdp_data;
	pdp_data = (struct ape_cmd *) data;
	/**
	 * enable all sub-modules
	 */
	if (pdp_data->rdma_need) {
		reg = FIELD_ENUM(PDP_RDMA_D_OP_ENABLE_0, OP_EN, ENABLE);
		pdp_rdma_reg_write(D_OP_ENABLE, reg);
	}
	reg = FIELD_ENUM(PDP_D_OP_ENABLE_0, OP_EN, ENABLE);
	pdp_reg_write(D_OP_ENABLE, reg); LOG_DEBUG("exit \n");  
}

static ScU32
get_fly_mode(ScU8 type)
{
	ScU32 val;

	val = type == APE_MEM_HW ?
			FIELD_ENUM(PDP_D_OPERATION_MODE_CFG_0,
						FLYING_MODE, ON_FLYING) :
			FIELD_ENUM(PDP_D_OPERATION_MODE_CFG_0,
						FLYING_MODE, OFF_FLYING);

	return val;
}

void pdp_program(void * data) {
    LOG_DEBUG("enter\n");
    ScS32 ret = 0;
	ScU32 reg, high, low;
	ScU64 input_address = 0;
	ScU64 output_address = 0;
	struct pdp_op_desc *pdp_op;
	struct pdp_surface_desc *pdp_surface;

    struct ape_cmd * pdp_data;
    pdp_data = (struct ape_cmd *)data;

	pdp_op = (struct pdp_op_desc *)pdp_data->op_addr;
	pdp_surface = (struct pdp_surface_desc *)pdp_data->surface_addr;

	ret = vaildate_pdp_configs(pdp_data);
	if (ret)
		goto exit;
 LOG_DEBUG("enter\n");
	ret = ape_read_input_address(&pdp_surface->src_data,
					&input_address);
					//group->op_desc->index,
					//group->roi_index,
					//1);

	 LOG_DEBUG("enter\n");
	if (pdp_surface->dst_data.address != -1)
		ape_get_dma_cube_address(NULL,NULL,
					pdp_surface->dst_data.address,
					pdp_surface->dst_data.offset,
					(void *)&output_address,
					DESTINATION_DMA);

	if (pdp_surface->src_data.type != APE_MEM_HW) {
		/* PDP RDMA */
		pdp_rdma_reg_write(D_DATA_CUBE_IN_WIDTH,
				pdp_surface->src_data.width - 1);
		pdp_rdma_reg_write(D_DATA_CUBE_IN_HEIGHT,
				pdp_surface->src_data.height - 1);
		pdp_rdma_reg_write(D_DATA_CUBE_IN_CHANNEL,
				pdp_surface->src_data.channel - 1);

		high = HIGH32BITS(input_address);
		low  = LOW32BITS(input_address);
		pdp_rdma_reg_write(D_SRC_BASE_ADDR_HIGH, high);
		pdp_rdma_reg_write(D_SRC_BASE_ADDR_LOW, low);
		pdp_rdma_reg_write(D_SRC_LINE_STRIDE,
				pdp_surface->src_data.line_stride);
		pdp_rdma_reg_write(D_SRC_SURFACE_STRIDE,
				pdp_surface->src_data.surf_stride);

		reg = (map_precision[pdp_op->precision]
			<< SHIFT(PDP_RDMA_D_DATA_FORMAT_0, INPUT_DATA));
		pdp_rdma_reg_write(D_DATA_FORMAT, reg);

		reg = map_ram[pdp_surface->src_data.type]
			<< SHIFT(PDP_RDMA_D_SRC_RAM_CFG_0, SRC_RAM_TYPE);
		pdp_rdma_reg_write(D_SRC_RAM_CFG, reg);

		reg = ((pdp_op->split_num - 1)
			 << SHIFT(PDP_RDMA_D_OPERATION_MODE_CFG_0, SPLIT_NUM));
		pdp_rdma_reg_write(D_OPERATION_MODE_CFG, reg);

		reg = (map_pool_kernel[pdp_op->pool_width]
			<< SHIFT(PDP_RDMA_D_POOLING_KERNEL_CFG_0,
							KERNEL_WIDTH)) |
			((pdp_op->stride_x - 1)
			<< SHIFT(PDP_RDMA_D_POOLING_KERNEL_CFG_0,
							KERNEL_STRIDE_WIDTH));
		pdp_rdma_reg_write(D_POOLING_KERNEL_CFG, reg);

		reg = (pdp_op->pad_left
			<< SHIFT(PDP_RDMA_D_POOLING_PADDING_CFG_0, PAD_WIDTH));
		pdp_rdma_reg_write(D_POOLING_PADDING_CFG, reg);

		reg = ((pdp_op->partial_in_width_first == 0 ? 0 :
				pdp_op->partial_in_width_first - 1)
			<< SHIFT(PDP_RDMA_D_PARTIAL_WIDTH_IN_0,
				PARTIAL_WIDTH_IN_FIRST)) |
			((pdp_op->partial_in_width_mid == 0 ? 0 :
				pdp_op->partial_in_width_mid - 1)
			<< SHIFT(PDP_RDMA_D_PARTIAL_WIDTH_IN_0,
				PARTIAL_WIDTH_IN_MID)) |
			((pdp_op->partial_in_width_last == 0 ? 0 :
				pdp_op->partial_in_width_last - 1)
			<< SHIFT(PDP_RDMA_D_PARTIAL_WIDTH_IN_0,
				PARTIAL_WIDTH_IN_LAST));
		pdp_rdma_reg_write(D_PARTIAL_WIDTH_IN, reg);
	} else {
		ASSERT_GOTO(pdp_op->split_num == 1, ret,
					not_initialized , exit);
	}

	reg = ((pdp_surface->src_data.width - 1)
		<< SHIFT(PDP_D_DATA_CUBE_IN_WIDTH_0, CUBE_IN_WIDTH));
	pdp_reg_write(D_DATA_CUBE_IN_WIDTH, reg);

	reg = ((pdp_surface->src_data.height - 1)
		<< SHIFT(PDP_D_DATA_CUBE_IN_HEIGHT_0, CUBE_IN_HEIGHT));
	pdp_reg_write(D_DATA_CUBE_IN_HEIGHT, reg);

	reg = ((pdp_surface->src_data.channel - 1)
		<< SHIFT(PDP_D_DATA_CUBE_IN_CHANNEL_0, CUBE_IN_CHANNEL));
	pdp_reg_write(D_DATA_CUBE_IN_CHANNEL, reg);

	reg = ((pdp_surface->dst_data.width - 1)
		<< SHIFT(PDP_D_DATA_CUBE_OUT_WIDTH_0, CUBE_OUT_WIDTH));
	pdp_reg_write(D_DATA_CUBE_OUT_WIDTH, reg);

	reg = ((pdp_surface->dst_data.height - 1)
		<< SHIFT(PDP_D_DATA_CUBE_OUT_HEIGHT_0, CUBE_OUT_HEIGHT));
	pdp_reg_write(D_DATA_CUBE_OUT_HEIGHT, reg);

	reg = ((pdp_surface->dst_data.channel - 1)
		<< SHIFT(PDP_D_DATA_CUBE_OUT_CHANNEL_0, CUBE_OUT_CHANNEL));
	pdp_reg_write(D_DATA_CUBE_OUT_CHANNEL, reg);

	reg = (map_pool[pdp_op->pool_mode]
		<< SHIFT(PDP_D_OPERATION_MODE_CFG_0, POOLING_METHOD)) |
		(get_fly_mode(pdp_surface->src_data.type)
		<< SHIFT(PDP_D_OPERATION_MODE_CFG_0, FLYING_MODE)) |
		((pdp_op->split_num - 1)
		<< SHIFT(PDP_D_OPERATION_MODE_CFG_0, SPLIT_NUM));
	pdp_reg_write(D_OPERATION_MODE_CFG, reg);

	reg = ((pdp_op->partial_in_width_first == 0 ? 0 :
			pdp_op->partial_in_width_first-1)
		<< SHIFT(PDP_D_PARTIAL_WIDTH_IN_0, PARTIAL_WIDTH_IN_FIRST)) |
		((pdp_op->partial_in_width_mid == 0 ? 0 :
			pdp_op->partial_in_width_mid-1)
		<< SHIFT(PDP_D_PARTIAL_WIDTH_IN_0, PARTIAL_WIDTH_IN_MID)) |
		((pdp_op->partial_in_width_last == 0 ? 0 :
			pdp_op->partial_in_width_last-1)
		<< SHIFT(PDP_D_PARTIAL_WIDTH_IN_0, PARTIAL_WIDTH_IN_LAST));
	pdp_reg_write(D_PARTIAL_WIDTH_IN, reg);

	reg = ((pdp_op->partial_width_first == 0 ? 0 :
			pdp_op->partial_width_first-1)
		<< SHIFT(PDP_D_PARTIAL_WIDTH_OUT_0, PARTIAL_WIDTH_OUT_FIRST)) |
		((pdp_op->partial_width_mid == 0 ? 0 :
			pdp_op->partial_width_mid-1)
		<< SHIFT(PDP_D_PARTIAL_WIDTH_OUT_0, PARTIAL_WIDTH_OUT_MID))   |
		((pdp_op->partial_width_last == 0 ? 0 :
			pdp_op->partial_width_last-1)
		<< SHIFT(PDP_D_PARTIAL_WIDTH_OUT_0, PARTIAL_WIDTH_OUT_LAST));
	pdp_reg_write(D_PARTIAL_WIDTH_OUT, reg);

	reg = (map_pool_kernel[pdp_op->pool_width]
		<< SHIFT(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_WIDTH)) |
		(map_pool_kernel[pdp_op->pool_height]
		<< SHIFT(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_HEIGHT))|
		((pdp_op->stride_x - 1)
		<< SHIFT(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_STRIDE_WIDTH)) |
		((pdp_op->stride_y - 1)
		<< SHIFT(PDP_D_POOLING_KERNEL_CFG_0, KERNEL_STRIDE_HEIGHT));
	pdp_reg_write(D_POOLING_KERNEL_CFG, reg);

	pdp_reg_write(D_RECIP_KERNEL_WIDTH,
			recip_kernel_size[pdp_op->precision ==
					PRECISION_FP16][pdp_op->pool_width]);
	pdp_reg_write(D_RECIP_KERNEL_HEIGHT,
			recip_kernel_size[pdp_op->precision ==
					PRECISION_FP16][pdp_op->pool_height]);

	reg = (pdp_op->pad_left
		<< SHIFT(PDP_D_POOLING_PADDING_CFG_0, PAD_LEFT)) |
		(pdp_op->pad_right
		<< SHIFT(PDP_D_POOLING_PADDING_CFG_0, PAD_RIGHT)) |
		(pdp_op->pad_top
		<< SHIFT(PDP_D_POOLING_PADDING_CFG_0, PAD_TOP)) |
		(pdp_op->pad_bottom
		<< SHIFT(PDP_D_POOLING_PADDING_CFG_0, PAD_BOTTOM));
	if (pdp_op->precision == PRECISION_FP16) {
		ScS32 i;

		for (i = 0; i < 7; i++)
			ASSERT_GOTO(pdp_op->padding_value[i] == 0, ret,
						not_initialized , exit);
	}

	pdp_reg_write(D_POOLING_PADDING_CFG, reg);
	pdp_reg_write(D_POOLING_PADDING_VALUE_1_CFG, pdp_op->padding_value[0]);
	pdp_reg_write(D_POOLING_PADDING_VALUE_2_CFG, pdp_op->padding_value[1]);
	pdp_reg_write(D_POOLING_PADDING_VALUE_3_CFG, pdp_op->padding_value[2]);
	pdp_reg_write(D_POOLING_PADDING_VALUE_4_CFG, pdp_op->padding_value[3]);
	pdp_reg_write(D_POOLING_PADDING_VALUE_5_CFG, pdp_op->padding_value[4]);
	pdp_reg_write(D_POOLING_PADDING_VALUE_6_CFG, pdp_op->padding_value[5]);
	pdp_reg_write(D_POOLING_PADDING_VALUE_7_CFG, pdp_op->padding_value[6]);

	if (pdp_surface->src_data.type != APE_MEM_HW) {
		pdp_reg_write(D_SRC_LINE_STRIDE,
				pdp_surface->src_data.line_stride);
		pdp_reg_write(D_SRC_SURFACE_STRIDE,
				pdp_surface->src_data.surf_stride);
	}

	high = HIGH32BITS(output_address);
	low = LOW32BITS(output_address);
	pdp_reg_write(D_DST_BASE_ADDR_LOW, low);
	pdp_reg_write(D_DST_BASE_ADDR_HIGH, high);

	pdp_reg_write(D_DST_LINE_STRIDE, pdp_surface->dst_data.line_stride);
	pdp_reg_write(D_DST_SURFACE_STRIDE, pdp_surface->dst_data.surf_stride);

	reg = (map_ram[pdp_surface->dst_data.type]
		<< SHIFT(PDP_D_DST_RAM_CFG_0, DST_RAM_TYPE));
	pdp_reg_write(D_DST_RAM_CFG, reg);

	reg = (map_precision[pdp_op->precision]
		<< SHIFT(PDP_D_DATA_FORMAT_0, INPUT_DATA));
	pdp_reg_write(D_DATA_FORMAT, reg);
    LOG_DEBUG("exit \n");
exit:
	return;

}

void pdp_set_hw_idx(void * data) {
    LOG_DEBUG("enter\n");
	ScU32 reg;
	struct ape_cmd *  pdp_data;
	pdp_data = (struct ape_cmd *) data;
	
	reg = pdp_data->pingpong_id << SHIFT(PDP_S_POINTER_0,PRODUCER);
	pdp_reg_write(S_POINTER, reg);

	reg = pdp_data->pingpong_id << SHIFT(PDP_RDMA_S_POINTER_0, PRODUCER);
	pdp_rdma_reg_write(S_POINTER, reg);
    LOG_DEBUG("exit \n");
}
void pdp_parse(void * data) {
    LOG_DEBUG("enter\n");
    struct ape_cmd * pdp_data ;
    struct pdp_surface_desc * pdp_surf;
    struct pdp_op_desc * pdp_op;

    pdp_data = (struct ape_cmd *) data;
    pdp_surf = (struct pdp_surface_desc *) pdp_data->surface_addr;
    pdp_op = (struct pdp_op_desc *) pdp_data->op_addr;
    
    ape_debug_pdp_surface_desc(pdp_surf,1);
    ape_debug_pdp_op_desc(pdp_op,1);
    LOG_DEBUG("exit \n");
}


void pdp_finish(void *data) {
    
    int hw_idx;
    struct ape_cmd *pdp_data;
    pdp_data = (struct ape_cmd *)data;
    
    hw_idx = pdp_data->pingpong_id;
    unsigned int field =  0x3<<(16*hw_idx);
    unsigned int expect_value =  0x1<<(16*hw_idx);
    poll_field_not_equal(PDP_RDMA_S_STATUS_0, field, expect_value);
    poll_field_not_equal(PDP_S_STATUS_0, field, expect_value);
}