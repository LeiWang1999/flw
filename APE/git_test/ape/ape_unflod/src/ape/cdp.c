#include "cdp.h"
#include "log.h"
#include "ape_error.h"
#include "ape_types.h"
#include "ape_interface.h"
#include "common.h"
#include "ape_engine_internal.h"
#include "ape_initial.h"
#include "ape_debug.h"

static const uint8_t map_ram[] = {
	FIELD_ENUM(CDP_RDMA_D_SRC_DMA_CFG_0, SRC_RAM_TYPE, MC),
	FIELD_ENUM(CDP_RDMA_D_SRC_DMA_CFG_0, SRC_RAM_TYPE, CV),
};

static const uint8_t map_precision[] = {
	FIELD_ENUM(CDP_RDMA_D_DATA_FORMAT_0, INPUT_DATA, INT8),
	FIELD_ENUM(CDP_RDMA_D_DATA_FORMAT_0, INPUT_DATA, INT16),
	FIELD_ENUM(CDP_RDMA_D_DATA_FORMAT_0, INPUT_DATA, FP16),
};

static uint32_t
map_local_size(uint8_t local_size)
{
	return ((local_size-1)/2)-1;
}

void cdp_enable(void * data) {
    LOG_DEBUG("enter\n"); 
    uint32_t reg;
	//uint8_t perf_reg;
	//struct dla_engine *engine = dla_get_engine();

	/**
	 * enable all sub-modules
	 */
	reg = FIELD_ENUM(CDP_RDMA_D_OP_ENABLE_0, OP_EN, ENABLE);
	cdp_rdma_reg_write(D_OP_ENABLE, reg);
	reg = FIELD_ENUM(CDP_D_OP_ENABLE_0, OP_EN, ENABLE);
	cdp_reg_write(D_OP_ENABLE, reg);

	return;
}

void cdp_program(void * data) {
    LOG_DEBUG("enter\n");
    #if 0
    int32_t ret = 0;
	uint32_t reg, high, low;
	uint64_t input_address = 0;
	uint64_t output_address = 0;
	struct ape_lut_param lut;
	//struct dla_engine *engine = dla_get_engine();
	struct cdp_op_desc *cdp_op;
	struct cdp_surface_desc *cdp_surface;

	struct ape_cmd * cdp_data;
    cdp_data = (struct ape_cmd *)data; 

	cdp_op = cdp_data->op_addr;
	cdp_surface = cdp_data->surface_addr;

	/* Argument check */
	if (cdp_surface->src_data.type == DLA_MEM_HW) {
		//dla_error("Invalid source memory type\n");
		ret = not_initialized;
		goto exit;
	}
	if (cdp_surface->dst_data.type == DLA_MEM_HW) {
		//dla_error("Invalid destination memory type\n");
		ret = not_initialized;
		goto exit;
	}

	if (cdp_op->in_precision != cdp_op->out_precision) {
		//dla_error("CDP does not support precision conversion\n");
		ret = not_initialized;
		goto exit;
	}

	/* get the addresses from task descriptor */
	ret = dla_read_input_address(&cdp_surface->src_data,
						&input_address);
						//group->op_desc->index,
						//group->roi_index,
						//1);
	if (ret)
		goto exit;

	dla_get_dma_cube_address(NULL,NULL,
				cdp_surface->dst_data.address,
				cdp_surface->dst_data.offset,
				(void *)&output_address,
				DESTINATION_DMA);
    /*   ´ýÓÃ */
	/*if (cdp_op->lut_index >= 0) {
		group->lut_index = cdp_op->lut_index;
		dla_read_lut(engine, cdp_op->lut_index, (void *)&lut);
		dla_debug_lut_params(&lut);
	}*/

	/* config CDP RDMA registers */
	reg = ((cdp_surface->src_data.width - 1)
		<< SHIFT(CDP_RDMA_D_DATA_CUBE_WIDTH_0, WIDTH));
	cdp_rdma_reg_write(D_DATA_CUBE_WIDTH, reg);

	reg = ((cdp_surface->src_data.height - 1)
		<< SHIFT(CDP_RDMA_D_DATA_CUBE_HEIGHT_0, HEIGHT));
	cdp_rdma_reg_write(D_DATA_CUBE_HEIGHT, reg);

	reg = ((cdp_surface->src_data.channel - 1)
		<< SHIFT(CDP_RDMA_D_DATA_CUBE_CHANNEL_0, CHANNEL));
	cdp_rdma_reg_write(D_DATA_CUBE_CHANNEL, reg);

	high = HIGH32BITS(input_address);
	low = LOW32BITS(input_address);
	cdp_rdma_reg_write(D_SRC_BASE_ADDR_LOW, low);
	cdp_rdma_reg_write(D_SRC_BASE_ADDR_HIGH, high);

	cdp_rdma_reg_write(D_SRC_LINE_STRIDE,
			cdp_surface->src_data.line_stride);
	cdp_rdma_reg_write(D_SRC_SURFACE_STRIDE,
			cdp_surface->src_data.surf_stride);

	reg = (map_ram[cdp_surface->src_data.type]
		<< SHIFT(CDP_RDMA_D_SRC_DMA_CFG_0, SRC_RAM_TYPE));
	cdp_rdma_reg_write(D_SRC_DMA_CFG, reg);

	reg = (map_precision[cdp_op->in_precision]
		<< SHIFT(CDP_RDMA_D_DATA_FORMAT_0, INPUT_DATA));
	cdp_rdma_reg_write(D_DATA_FORMAT, reg);

	/* config CDP */
	//if (cdp_op->lut_index >= 0)
	//	update_lut(CDP_S_LUT_ACCESS_CFG_0, &lut, cdp_op->in_precision);´ýÒé

	high = HIGH32BITS(output_address);
	low = LOW32BITS(output_address);
	cdp_reg_write(D_DST_BASE_ADDR_LOW, low);
	cdp_reg_write(D_DST_BASE_ADDR_HIGH, high);

	cdp_reg_write(D_DST_LINE_STRIDE, cdp_surface->dst_data.line_stride);
	cdp_reg_write(D_DST_SURFACE_STRIDE, cdp_surface->dst_data.surf_stride);

	reg = (map_ram[cdp_surface->dst_data.type]
		<< SHIFT(CDP_D_DST_DMA_CFG_0, DST_RAM_TYPE));
	cdp_reg_write(D_DST_DMA_CFG, reg);

	reg = (map_precision[cdp_op->in_precision]
		<< SHIFT(CDP_D_DATA_FORMAT_0, INPUT_DATA_TYPE));
	cdp_reg_write(D_DATA_FORMAT, reg);

	reg = (map_local_size(cdp_op->local_size)
		<< SHIFT(CDP_D_LRN_CFG_0, NORMALZ_LEN));
	cdp_reg_write(D_LRN_CFG, reg);

	reg = (cdp_op->in_cvt.offset
		<< SHIFT(CDP_D_DATIN_OFFSET_0, DATIN_OFFSET));
	cdp_reg_write(D_DATIN_OFFSET, reg);

	reg = (cdp_op->in_cvt.scale
		<< SHIFT(CDP_D_DATIN_SCALE_0, DATIN_SCALE));
	cdp_reg_write(D_DATIN_SCALE, reg);

	reg = (cdp_op->in_cvt.truncate
		<< SHIFT(CDP_D_DATIN_SHIFTER_0, DATIN_SHIFTER));
	cdp_reg_write(D_DATIN_SHIFTER, reg);

	reg = (cdp_op->out_cvt.offset
		<< SHIFT(CDP_D_DATOUT_OFFSET_0, DATOUT_OFFSET));
	cdp_reg_write(D_DATOUT_OFFSET, reg);

	reg = (cdp_op->out_cvt.scale
		<< SHIFT(CDP_D_DATOUT_SCALE_0, DATOUT_SCALE));
	cdp_reg_write(D_DATOUT_SCALE, reg);

	reg = (cdp_op->out_cvt.truncate
		<< SHIFT(CDP_D_DATOUT_SHIFTER_0, DATOUT_SHIFTER));
	cdp_reg_write(D_DATOUT_SHIFTER, reg);

	reg = ((cdp_op->bypass_sqsum ?
		FIELD_ENUM(CDP_D_FUNC_BYPASS_0, SQSUM_BYPASS, ENABLE) :
		FIELD_ENUM(CDP_D_FUNC_BYPASS_0, SQSUM_BYPASS, DISABLE)) <<
		SHIFT(CDP_D_FUNC_BYPASS_0, SQSUM_BYPASS)) |
		((cdp_op->bypass_out_mul ?
		FIELD_ENUM(CDP_D_FUNC_BYPASS_0, MUL_BYPASS, ENABLE) :
		FIELD_ENUM(CDP_D_FUNC_BYPASS_0, MUL_BYPASS, DISABLE)) <<
		SHIFT(CDP_D_FUNC_BYPASS_0, MUL_BYPASS));
	cdp_reg_write(D_FUNC_BYPASS, reg);

exit:
	return;
    #endif
}

void cdp_set_hw_idx(void * data) {
    LOG_DEBUG("enter\n");
    uint32_t reg;
    struct ape_cmd *  cdp_data;
	cdp_data = (struct ape_cmd *) data;

	/**
	 * set producer pointer for all sub-modules
	 */
    
	reg = cdp_data->pingpong_id << SHIFT(CDP_S_POINTER_0, PRODUCER);
	cdp_reg_write(S_POINTER, reg);
	reg = cdp_data->pingpong_id << SHIFT(CDP_RDMA_S_POINTER_0, PRODUCER);
	cdp_rdma_reg_write(S_POINTER, reg);
}

void cdp_parse(void * data) {
    LOG_DEBUG("enter\n");
    struct cdp_op_desc *cdp_op;
	struct cdp_surface_desc *cdp_surface;

    struct ape_cmd * cdp_data;
    cdp_data = (struct ape_cmd *)data; 

	cdp_surface = cdp_data->surface_addr;
	cdp_op = cdp_data->op_addr;

	dla_debug_cdp_surface_desc(cdp_surface, 1);
	dla_debug_cdp_op_desc(cdp_op, 1);
}