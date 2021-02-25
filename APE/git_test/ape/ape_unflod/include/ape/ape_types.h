#ifndef _APE_TYPES_H
#define _APE_TYPES_H
#include <stdint.h>


#pragma pack (4)


#define FORMAT_T_R8			0
#define FORMAT_T_R10			1
#define FORMAT_T_R12			2
#define FORMAT_T_R16			3
#define FORMAT_T_R16_I			4
#define FORMAT_T_R16_F			5
#define FORMAT_T_A16B16G16R16		6
#define FORMAT_T_X16B16G16R16		7
#define FORMAT_T_A16B16G16R16_F		8
#define FORMAT_T_A16Y16U16V16		9
#define FORMAT_T_V16U16Y16A16		10
#define FORMAT_T_A16Y16U16V16_F		11
#define FORMAT_T_A8B8G8R8		12
#define FORMAT_T_A8R8G8B8		13
#define FORMAT_T_B8G8R8A8		14
#define FORMAT_T_R8G8B8A8		15
#define FORMAT_T_X8B8G8R8		16
#define FORMAT_T_X8R8G8B8		17
#define FORMAT_T_B8G8R8X8		18
#define FORMAT_T_R8G8B8X8		19
#define FORMAT_T_A2B10G10R10		20
#define FORMAT_T_A2R10G10B10		21
#define FORMAT_T_B10G10R10A2		22
#define FORMAT_T_R10G10B10A2		23
#define FORMAT_T_A2Y10U10V10		24
#define FORMAT_T_V10U10Y10A2		25
#define FORMAT_T_A8Y8U8V8			26
#define FORMAT_T_V8U8Y8A8			27
#define FORMAT_T_Y8___U8V8_N444		28
#define FORMAT_T_Y8___V8U8_N444		29
#define FORMAT_T_Y10___U10V10_N444	30
#define FORMAT_T_Y10___V10U10_N444	31
#define FORMAT_T_Y12___U12V12_N444	32
#define FORMAT_T_Y12___V12U12_N444	33
#define FORMAT_T_Y16___U16V16_N444	34
#define FORMAT_T_Y16___V16U16_N444	35
#define FORMAT_FEATURE			36


#define DLA_MEM_MC			0 /* External DRAM */
#define DLA_MEM_CV			1 /* CV-SRAM */
#define DLA_MEM_HW			2 /* DLA sub-module */
struct cvt_param {
	int16_t  scale;
	uint8_t  truncate;
	uint8_t  enable;

	int32_t  offset;
};

struct ape_roi_array_desc {
	uint32_t array_length;

	uint32_t array_reserved;
} ;

struct ape_roi_desc {
	uint32_t left;

	uint32_t top;

	uint32_t right;

	uint32_t bottom;
} ;

struct data_cube {
	uint16_t type; /* mem_type */
	int16_t address; /* offset to the actual IOVA in task.address_list */

	uint32_t offset; /* offset within address */
	uint32_t size;

	/* cube dimensions */
	uint16_t width;
	uint16_t height;

	uint16_t channel;
	uint16_t reserved0;

	/* stride information */
	uint32_t line_stride;
	uint32_t surf_stride;

	/* For Rubik only */
	uint32_t plane_stride;
};




#define CONV_MODE_DIRECT	0
#define CONV_MODE_WINOGRAD	1
struct conv_op_desc {
	/* Performance parameters */

	/* conv_mode */
	uint8_t conv_mode;
	uint8_t data_reuse;
	uint8_t weight_reuse;
	uint8_t skip_data_rls;

	uint8_t skip_weight_rls;
	uint8_t reserved0;
	uint16_t entry_per_slice;

	/* data_format */
	uint8_t data_format;
	/* pixel_mapping */
	uint8_t pixel_mapping;
	/* number of free slices before fetch */
	uint16_t fetch_grain;

	uint8_t reserved_b[8];

	/* batch_num */
	uint8_t batch;
	/* weight_format */
	uint8_t weight_format;
	uint8_t data_bank;
	uint8_t weight_bank;

	/* the offset in bytes of each data cube in a batch */
	uint32_t batch_stride;

	uint8_t post_extension;
	uint8_t pixel_override;
	/* number of slices need to be released */
	uint16_t release;

	 /* The input cube dimension for CSC */
	uint16_t input_width_csc;
	uint16_t input_height_csc;

	uint16_t input_channel_csc;
	uint16_t kernel_width_csc;

	uint16_t kernel_height_csc;
	uint16_t kernel_channel_csc;

	/* The input cube dimension for CMAC */
	uint16_t input_width_cmac;
	uint16_t input_height_cmac;

	/* actual size in bytes */
	uint32_t bytes_per_kernel;

	/* Algorithm parameters */

	int16_t mean_ry; /* mean value for red in RGB or Y in YUV */
	int16_t mean_gu; /* mean value for green in RGB or U in YUV */

	int16_t mean_bv; /* mean value for blue in RGB or V in YUV */
	int16_t mean_ax;

	uint8_t mean_format; /* mean_format */
	uint8_t conv_stride_x;
	uint8_t conv_stride_y;
	uint8_t pad_x_left;

	uint8_t pad_x_right;
	uint8_t pad_y_top;
	uint8_t pad_y_bottom;
	uint8_t dilation_x;

	uint8_t dilation_y;
	uint8_t reserved2[2];

	/* Precision parameters */
	uint8_t pra_truncate;

	uint8_t in_precision;
	/* The output precision from CONV, it's the MAC processing precison */
	uint8_t out_precision;
	int16_t pad_val;

	/* input converter parameters */
	struct cvt_param in_cvt;
	/* output converter parameters, support truncate only */
	struct cvt_param out_cvt;

}; 

struct conv_surface_desc {
	/* Data cube */
	struct data_cube weight_data;
	struct data_cube wmb_data;
	struct data_cube wgs_data;
	struct data_cube src_data;
	struct data_cube dst_data;

	/**
	 * u_addr = input_data.source_addr + offset_u
	 * this field should be set when YUV is not interleave format
	 *
	 */
	int64_t offset_u;

	/* line stride for 2nd plane, must be 32bytes aligned */
	uint32_t in_line_uv_stride;
} ;

struct conv_stat_desc {
	uint32_t data_read_stall;
	uint32_t weight_read_stall;
	uint32_t data_read_latency;
	uint32_t weight_read_latency;
	uint32_t saturation_count;
	uint32_t nan_data_num;
	uint32_t nan_weight_num;
	uint32_t inf_data_num;
	uint32_t inf_weight_num;
	uint32_t runtime;
};

struct sdp_surface_desc {
	/* Data cube */
	/* source input cube, available when SDP working on offline mode */
	struct data_cube src_data;

	/* X1 input cube */
	struct data_cube x1_data;

	/* X2 input cube */
	struct data_cube x2_data;

	/* Y input cube */
	struct data_cube y_data;

	/* Output cube */
	struct data_cube dst_data;
};
#define SDP_OP_NONE		0
#define SDP_OP_MUL		1
#define SDP_OP_ADD		2
#define SDP_OP_BOTH		3

#define SDP_ALU_OP_MAX		0
#define SDP_ALU_OP_MIN		1
#define SDP_ALU_OP_SUM		2
#define SDP_ALU_OP_EQL		3

#define SDP_OP_PER_LAYER	0
#define SDP_OP_PER_KERNEL	1
#define SDP_OP_PER_POINT	2
struct sdp_cvt {
	struct cvt_param alu_cvt;
	struct cvt_param mul_cvt;
};

struct sdp_op {
	uint8_t enable;
	uint8_t alu_type; /* sdp_alu_op_type */
	uint8_t type; /* sdp_op_type */
	uint8_t mode; /* sdp_op_mode */

	uint8_t act; /* act_type */
	uint8_t shift_value; /* left shift */
	uint8_t truncate;
	uint8_t precision;

	int32_t alu_operand;
	int32_t mul_operand;

	struct sdp_cvt  cvt;
};

struct sdp_op_desc {
	/* Precision parameters */
	/* precision */
	uint8_t src_precision;
	uint8_t dst_precision;
	int16_t lut_index;

	struct cvt_param out_cvt;

	/* Performance parameters */
	/* conv_mode */
	uint8_t conv_mode;
	uint8_t batch_num;
	uint16_t reserved0;

	uint32_t batch_stride;	/* will be used when batch_num > 1 */

	/* Algorithm parameters */
	struct sdp_op x1_op;
	struct sdp_op x2_op;
	struct sdp_op y_op;
};

struct sdp_stat_desc {
	uint32_t nan_input_num;
	uint32_t inf_input_num;
	uint32_t nan_output_num;
	uint32_t wdma_write_stall;
	uint32_t lut_underflow;
	uint32_t lut_overflow;
	uint32_t lut_hybrid;
	uint32_t lut_le_hit;
	uint32_t lut_lo_hit;
	uint32_t saturation_count;
	uint32_t runtime;
};


#define ACTIVATION_NONE		0
#define ACTIVATION_RELU		1
#define ACTIVATION_LUT		2
#define ACTIVATION_PRELU	3
/** @} */

/**
 * @ingroup LUT
 * @name LUT size
 * @brief LUT sizes for linear and exponentila LUT
 * @{
 */
#define LUT_LINEAR_EXP_TABLE_ENTRY_LOG2		6
#define LUT_LINEAR_ONLY_TABLE_ENTRY_LOG2	8
/** @} */

/**
 * @ingroup LUT
 * @name LUT types
 * @brief ape supports two types of LUT, linear and exonential
 * @{
 */
#define LUT_LINEAR_EXP_TABLE		0
#define LUT_LINEAR_ONLY_TABLE		1
/** @} */

/**
 * @ingroup LUT
 * @name LUT methods
 * @brief ape supports two types of LUT, linear and exonential
 * @{
 */
#define LUT_METHOD_EXPONENTIAL		0
#define LUT_METHOD_LINEAR		1
/** @} */

/**
 * @ingroup LUT
 * @name LUT
 * @brief ape supports two types of LUT, linear and exonential
 * @{
 */
#define LUT_PRI_LINEAR_EXP		0
#define LUT_PRI_LINEAR_ONLY		1
/** @} */

union ape_lut_offset {
	/**
	 * Number should be substracted on log domain before look up
	 * exponetial table it has the same definition as hardware
	 * thus input scaling should also take into account when
	 * set this field.
	 */
	int8_t exp_offset;
	/**
	 * Number of bits should be right shift before looking
	 * up linear table
	 */
	int8_t frac_bits;
	uint16_t reserved0;
};

/**
 * This struct is used to represent floating point values by INT
 * suppose we have a float point number fp_x, it will be represented
 * as:
 *
 * fp_x = scale_int_x>>(shifter_x)
 *
 * This is very useful for INT pipeline;
 */
struct ape_float_data {
	int16_t scale;
	int8_t shifter;
	uint8_t reserved0;
};

/**
 * For INT pipeline, we use the struct above to represent a floating number;
 * For FP16 pipeline, we should store the FP16 encoded value into a uint16_t
 * container
 */
union ape_slope {
	struct ape_float_data data_i;

	uint16_t data_f;
};

struct ape_lut_param {
	/**
	 * value of expression ((1<<LUT_LINEAR_EXP_TABLE_ENTRY_LOG2)+1) is 65,
	 * ((1<<LUT_LINEAR_ONLY_TABLE_ENTRY_LOG2)+1) is 257, and int16_t is of
	 * 2Byte. And below two statement's combined memory size is 644 Byte.
	 *
	 * NOTE: below two declaration combined size should always be multiple
	 * of 4.
	 */
	int16_t linear_exp_table[(1<<LUT_LINEAR_EXP_TABLE_ENTRY_LOG2)+1];
	int16_t linear_only_table[(1<<LUT_LINEAR_ONLY_TABLE_ENTRY_LOG2)+1];

	union ape_lut_offset linear_exp_offset;
	union ape_lut_offset linear_only_offset;

	/**
	 * The start and end point of raw table,
	 * valid when raw_method=LINEAR only
	 */
	uint64_t linear_exp_start;
	uint64_t linear_exp_end;
	uint64_t linear_only_start;
	uint64_t linear_only_end;

	union ape_slope linear_exp_underflow_slope;
	union ape_slope linear_exp_overflow_slope;
	union ape_slope linear_only_underflow_slope;
	union ape_slope linear_only_overflow_slope;

	/**
	 * ape_lut_priority, when both lut are hit(or one overflow,
	 * the other underflow), which one should be selected as output
	 */
	uint8_t hybrid_priority;
	uint8_t underflow_priority;
	uint8_t overflow_priority;
	uint8_t method; /* ape_lut_method */
};

struct pdp_surface_desc {
	/* Data cube */
	struct data_cube src_data;

	struct data_cube dst_data;
};

#define PDP_PAD_VAL_NUM	7

struct pdp_op_desc {
	/* Performance parameters */
	uint16_t  partial_in_width_first;
	uint16_t  partial_in_width_mid;

	uint16_t  partial_in_width_last;
	uint16_t  partial_width_first;

	uint16_t  partial_width_mid;
	uint16_t  partial_width_last;

	uint8_t   split_num;

	/* Algorithm parameters */
	uint8_t  pool_mode; /* pool_mode */
	uint8_t  pool_width; /* pool_width */
	uint8_t  pool_height; /* pool_height */

	uint8_t  stride_x;
	uint8_t  stride_y;

	/**
	 * The left/right padding size,
	 * pad_right might be less than pad_left
	 */
	uint8_t  pad_left;
	uint8_t  pad_right;

	/* The top/bottom padding size */
	uint8_t  pad_top;
	uint8_t  pad_bottom;

	/* Precision parameters */
	uint8_t  precision; /* precision */
	uint8_t  reserved0;
	/**
	 * if input has non-zero "offset", this value should be set
	 * There'll be 7 different paddding values, the relationship between
	 * those versions are:
	 * padding_value[0] = -offset*scaling;
	 * padding_value[1] = 2*padding_value[0]
	 * padding_value[2] = 3*padding_value[0]
	 * ...
	 * The purpose is to avoid ucode implement FP16
	 * multiplier(for FP16 mode)
	 */
	int32_t  padding_value[PDP_PAD_VAL_NUM];
};

struct pdp_stat_desc {
	uint32_t inf_input_num;
	uint32_t nan_input_num;
	uint32_t nan_output_num;
	uint32_t write_stall;
	uint32_t runtime;
};

struct cdp_surface_desc {
	/* Data cube */
	struct data_cube src_data;

	struct data_cube dst_data;
};

struct cdp_op_desc {
	/* Precision parameters */

	/* precision */
	uint8_t  in_precision;
	uint8_t  out_precision;
	int16_t  lut_index;

	struct cvt_param in_cvt;
	struct cvt_param out_cvt;

	/* Performance parameters */

	/* Algorithm parameters */
	uint8_t  local_size;
	uint8_t  bypass_sqsum;
	uint8_t  bypass_out_mul;
	uint8_t  reserved0;
};

struct cdp_stat_desc {
	uint32_t nan_input_num;
	uint32_t inf_input_num;
	uint32_t nan_output_num;
	uint32_t write_stall;
	uint32_t lut_uflow;
	uint32_t lut_oflow;
	uint32_t lut_hybrid;
	uint32_t lut_le_hit;
	uint32_t lut_lo_hit;
	uint32_t saturation_count;
	uint32_t runtime;
};

struct rubik_surface_desc {
	/* Data cube */
	struct data_cube src_data;

	struct data_cube dst_data;
};


struct rubik_op_desc {
	/* Precision parameters */
	uint8_t mode;
	uint8_t precision;
	uint8_t stride_x;
	uint8_t stride_y;
};

struct rubik_stat_desc {
	uint32_t read_stall;
	uint32_t write_stall;
	uint32_t runtime;
};

#define NUM_MAX_BDMA_OPS	20
/** @} */

struct bdma_transfer_desc {
	int16_t source_address;
	int16_t destination_address;

	uint32_t line_size;

	uint32_t line_repeat;

	uint32_t source_line;

	uint32_t destination_line;

	uint32_t surface_repeat;

	uint32_t source_surface;

	uint32_t destination_surface;
};

struct bdma_surface_desc {
	uint8_t source_type;
	uint8_t destination_type;
	uint16_t num_transfers;

	struct bdma_transfer_desc transfers[NUM_MAX_BDMA_OPS];
};

struct bdma_op_desc {
	uint16_t num_transfers;
	uint16_t reserved0;
};

struct bdma_stat_desc {
	uint32_t read_stall;
	uint32_t write_stall;
	uint32_t runtime;
};


union surface_container {
	struct bdma_surface_desc bdma_surface;
	struct conv_surface_desc conv_surface;
	struct sdp_surface_desc sdp_surface;
	struct pdp_surface_desc pdp_surface;
	struct cdp_surface_desc cdp_surface;
	struct rubik_surface_desc rubik_surface;
}__attribute__ ((aligned (1)));

union operation_container {
	struct bdma_op_desc bdma_op;
	struct conv_op_desc conv_op;
	struct sdp_op_desc sdp_op;
	struct pdp_op_desc pdp_op;
	struct cdp_op_desc cdp_op;
	struct rubik_op_desc rubik_op;
};

#endif