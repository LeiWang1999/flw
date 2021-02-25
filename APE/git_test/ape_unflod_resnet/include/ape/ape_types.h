#ifndef _APE_TYPES_H
#define _APE_TYPES_H
#include <stdint.h>
#include <stdbool.h>
#if JPEG
#include "ape_type.h"
#endif
#include "ape_dev_if.h"

#pragma pack (4)

#if RM_JPEG

#define SC_TRUE 1
#define SC_FALSE 0	

typedef signed char ScS8 ;
typedef signed short ScS16 ;
typedef signed int ScS32 ;
typedef signed long long ScS64 ;

typedef unsigned char ScU8 ;
typedef unsigned short ScU16 ;
typedef unsigned int ScU32 ;
typedef unsigned long long ScU64 ;
typedef unsigned long  ScSU64 ;

typedef bool ScBool;

typedef float   ScF32;
typedef double  ScF64;

//typedef ScU64 ScUPtr;
#endif

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


#define APE_MEM_MC			0 /* External DRAM */
#define APE_MEM_CV			1 /* CV-SRAM */
#define APE_MEM_HW			2 /* APE sub-module */
struct cvt_param {
	ScS16  scale:16;
	ScU8  truncate:8;
	ScU8  enable:8;

	ScS32  offset:32;
};

struct ape_roi_array_desc {
	ScU32 array_length:32;

	ScU32 array_reserved:32;
} ;

struct ape_roi_desc {
	ScU32 left:32;

	ScU32 top:32;

	ScU32 right:32;

	ScU32 bottom:32;
} ;

struct data_cube {
	ScU16 type:16; /* mem_type */
	ScS16 address:16; /* offset to the actual IOVA in task.address_list */

	ScU32 offset:32; /* offset within address */
	ScU32 size:32;

	/* cube dimensions */
	ScU16 width:16;
	ScU16 height:16;

	ScU16 channel:16;
	ScU16 reserved0:16;

	/* stride information */
	ScU32 line_stride:32;
	ScU32 surf_stride:32;

	/* For Rubik only */
	ScU32 plane_stride:32;
};




#define CONV_MODE_DIRECT	0
#define CONV_MODE_WINOGRAD	1
struct conv_op_desc {
	/* Performance parameters */

	/* conv_mode */
	ScU8 conv_mode:8;
	ScU8 data_reuse:8;
	ScU8 weight_reuse:8;
	ScU8 skip_data_rls:8;

	ScU8 skip_weight_rls:8;
	ScU8 reserved0:8;
	ScU16 entry_per_slice:16;

	/* data_format */
	ScU8 data_format:8;
	/* pixel_mapping */
	ScU8 pixel_mapping:8;
	/* number of free slices before fetch */
	ScU16 fetch_grain:16;

	//ScU8 reserved_b[8];
	ScU32 reserved_a:32;
	ScU32 reserved_b:32;
	
	/* batch_num */
	ScU8 batch:8;
	/* weight_format */
	ScU8 weight_format:8;
	ScU8 data_bank:8;
	ScU8 weight_bank:8;

	/* the offset in bytes of each data cube in a batch */
	ScU32 batch_stride:32;

	ScU8 post_extension:8;
	ScU8 pixel_override:8;
	/* number of slices need to be released */
	ScU16 release:16;

	 /* The input cube dimension for CSC */
	ScU16 input_width_csc:16;
	ScU16 input_height_csc:16;

	ScU16 input_channel_csc:16;
	ScU16 kernel_width_csc:16;

	ScU16 kernel_height_csc:16;
	ScU16 kernel_channel_csc:16;

	/* The input cube dimension for CMAC */
	ScU16 input_width_cmac:16;
	ScU16 input_height_cmac:16;

	/* actual size in bytes */
	ScU32 bytes_per_kernel:32;

	/* Algorithm parameters */

	ScS16 mean_ry:16; /* mean value for red in RGB or Y in YUV */
	ScS16 mean_gu:16; /* mean value for green in RGB or U in YUV */

	ScS16 mean_bv:16; /* mean value for blue in RGB or V in YUV */
	ScS16 mean_ax:16;

	ScU8 mean_format:8; /* mean_format */
	ScU8 conv_stride_x:8;
	ScU8 conv_stride_y:8;
	ScU8 pad_x_left:8;

	ScU8 pad_x_right:8;
	ScU8 pad_y_top:8;
	ScU8 pad_y_bottom:8;
	ScU8 dilation_x:8;

	ScU8 dilation_y:8;
	//ScU8 reserved2[2];
	ScU8 reserved2:8;
	ScU8 reserved1:8;

	/* Precision parameters */
	ScU8 pra_truncate:8;

	ScU8 in_precision:8;
	/* The output precision from CONV, it's the MAC processing precison */
	ScU8 out_precision:8;
	ScS16 pad_val:16;

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
	//ScS64 offset_u;
	ScS32 offset_u:32;
	ScS32 offset_u1:32;


	/* line stride for 2nd plane, must be 32bytes aligned */
	ScU32 in_line_uv_stride:32;
} ;

struct conv_stat_desc {
	ScU32 data_read_stall:32;
	ScU32 weight_read_stall:32;
	ScU32 data_read_latency:32;
	ScU32 weight_read_latency:32;
	ScU32 saturation_count:32;
	ScU32 nan_data_num:32;
	ScU32 nan_weight_num:32;
	ScU32 inf_data_num:32;
	ScU32 inf_weight_num:32;
	ScU32 runtime:32;
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
	ScU8 enable:8;
	ScU8 alu_type:8; /* sdp_alu_op_type */
	ScU8 type:8; /* sdp_op_type */
	ScU8 mode:8; /* sdp_op_mode */

	ScU8 act:8; /* act_type */
	ScU8 shift_value:8; /* left shift */
	ScU8 truncate:8;
	ScU8 precision:8;

	ScS32 alu_operand:32;
	ScS32 mul_operand:32;

	struct sdp_cvt  cvt;
};

struct sdp_op_desc {
	/* Precision parameters */
	/* precision */
	ScU8 src_precision:8;
	ScU8 dst_precision:8;
	ScS16 lut_index:16;

	struct cvt_param out_cvt;

	/* Performance parameters */
	/* conv_mode */
	ScU8 conv_mode:8;
	ScU8 batch_num:8;
	ScU16 reserved0:16;

	ScU32 batch_stride:32;	/* will be used when batch_num > 1 */

	/* Algorithm parameters */
	struct sdp_op x1_op;
	struct sdp_op x2_op;
	struct sdp_op y_op;
};

struct sdp_stat_desc {
	ScU32 nan_input_num:32;
	ScU32 inf_input_num:32;
	ScU32 nan_output_num:32;
	ScU32 wdma_write_stall:32;
	ScU32 lut_underflow:32;
	ScU32 lut_overflow:32;
	ScU32 lut_hybrid:32;
	ScU32 lut_le_hit:32;
	ScU32 lut_lo_hit:32;
	ScU32 saturation_count:32;
	ScU32 runtime:32;
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
	ScS8 exp_offset:8;
	/**
	 * Number of bits should be right shift before looking
	 * up linear table
	 */
	ScS8 frac_bits:8;
	ScU16 reserved0:16;
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
	ScS16 scale:16;
	ScS8 shifter:8;
	ScU8 reserved0:8;
};

/**
 * For INT pipeline, we use the struct above to represent a floating number;
 * For FP16 pipeline, we should store the FP16 encoded value into a ScU16
 * container
 */
union ape_slope {
	struct ape_float_data data_i;

	ScU16 data_f:16;
};

struct ape_lut_param {
	/**
	 * value of expression ((1<<LUT_LINEAR_EXP_TABLE_ENTRY_LOG2)+1) is 65,
	 * ((1<<LUT_LINEAR_ONLY_TABLE_ENTRY_LOG2)+1) is 257, and ScS16 is of
	 * 2Byte. And below two statement's combined memory size is 644 Byte.
	 *
	 * NOTE: below two declaration combined size should always be multiple
	 * of 4.
	 */
	//ScS16 linear_exp_table[(1<<LUT_LINEAR_EXP_TABLE_ENTRY_LOG2)+1];
	//ScS16 linear_only_table[(1<<LUT_LINEAR_ONLY_TABLE_ENTRY_LOG2)+1];

	union ape_lut_offset linear_exp_offset;
	union ape_lut_offset linear_only_offset;

	/**
	 * The start and end point of raw table,
	 * valid when raw_method=LINEAR only
	 */
	ScU64 linear_exp_start:64;
	ScU64 linear_exp_end:64;
	ScU64 linear_only_start:64;
	ScU64 linear_only_end:64;

	union ape_slope linear_exp_underflow_slope;
	union ape_slope linear_exp_overflow_slope;
	union ape_slope linear_only_underflow_slope;
	union ape_slope linear_only_overflow_slope;

	/**
	 * ape_lut_priority, when both lut are hit(or one overflow,
	 * the other underflow), which one should be selected as output
	 */
	ScU8 hybrid_priority:8;
	ScU8 underflow_priority:8;
	ScU8 overflow_priority:8;
	ScU8 method:8; /* ape_lut_method */
};

struct pdp_surface_desc {
	/* Data cube */
	struct data_cube src_data;

	struct data_cube dst_data;
};

#define PDP_PAD_VAL_NUM	7

struct pdp_op_desc {
	/* Performance parameters */
	ScU16  partial_in_width_first:16;
	ScU16  partial_in_width_mid:16;

	ScU16  partial_in_width_last:16;
	ScU16  partial_width_first:16;

	ScU16  partial_width_mid:16;
	ScU16  partial_width_last:16;

	ScU8   split_num:8;

	/* Algorithm parameters */
	ScU8  pool_mode:8; /* pool_mode */
	ScU8  pool_width:8; /* pool_width */
	ScU8  pool_height:8; /* pool_height */

	ScU8  stride_x:8;
	ScU8  stride_y:8;

	/**
	 * The left/right padding size,
	 * pad_right might be less than pad_left
	 */
	ScU8  pad_left:8;
	ScU8  pad_right:8;

	/* The top/bottom padding size */
	ScU8  pad_top:8;
	ScU8  pad_bottom:8;

	/* Precision parameters */
	ScU8  precision:8; /* precision */
	ScU8  reserved0:8;
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
	ScS32  padding_value[PDP_PAD_VAL_NUM];
};

struct pdp_stat_desc {
	ScU32 inf_input_num:32;
	ScU32 nan_input_num:32;
	ScU32 nan_output_num:32;
	ScU32 write_stall:32;
	ScU32 runtime:32;
};

struct cdp_surface_desc {
	/* Data cube */
	struct data_cube src_data;

	struct data_cube dst_data;
};

struct cdp_op_desc {
	/* Precision parameters */

	/* precision */
	ScU8  in_precision:8;
	ScU8  out_precision:8;
	ScS16  lut_index:16;

	struct cvt_param in_cvt;
	struct cvt_param out_cvt;

	/* Performance parameters */

	/* Algorithm parameters */
	ScU8  local_size:8;
	ScU8  bypass_sqsum:8;
	ScU8  bypass_out_mul:8;
	ScU8  reserved0:8;
};

struct cdp_stat_desc {
	ScU32 nan_input_num:32;
	ScU32 inf_input_num:32;
	ScU32 nan_output_num:32;
	ScU32 write_stall:32;
	ScU32 lut_uflow:32;
	ScU32 lut_oflow:32;
	ScU32 lut_hybrid:32;
	ScU32 lut_le_hit:32;
	ScU32 lut_lo_hit:32;
	ScU32 saturation_count:32;
	ScU32 runtime:32;
};

struct rubik_surface_desc {
	/* Data cube */
	struct data_cube src_data;

	struct data_cube dst_data;
};


struct rubik_op_desc {
	/* Precision parameters */
	ScU8 mode:8;
	ScU8 precision:8;
	ScU8 stride_x:8;
	ScU8 stride_y:8;
};

struct rubik_stat_desc {
	ScU32 read_stall:32;
	ScU32 write_stall:32;
	ScU32 runtime:32;
};

#define NUM_MAX_BDMA_OPS	20
/** @} */

struct bdma_transfer_desc {
	ScS16 source_address:16;
	ScS16 destination_address:16;

	ScU32 line_size:32;

	ScU32 line_repeat:32;

	ScU32 source_line:32;

	ScU32 destination_line:32;

	ScU32 surface_repeat:32;

	ScU32 source_surface:32;

	ScU32 destination_surface:32;
};

struct bdma_surface_desc {
	ScU8 source_type:8;
	ScU8 destination_type:8;
	ScU16 num_transfers:16;

	struct bdma_transfer_desc transfers[NUM_MAX_BDMA_OPS];
};

struct bdma_op_desc {
	ScU16 num_transfers:16;
	ScU16 reserved0:16;
};

struct bdma_stat_desc {
	ScU32 read_stall:32;
	ScU32 write_stall:32;
	ScU32 runtime:32;
};


union surface_container {
	struct bdma_surface_desc bdma_surface;
	struct conv_surface_desc conv_surface;
	struct sdp_surface_desc sdp_surface;
	struct pdp_surface_desc pdp_surface;
	struct cdp_surface_desc cdp_surface;
	struct rubik_surface_desc rubik_surface;
};

union operation_container {
	struct bdma_op_desc bdma_op;
	struct conv_op_desc conv_op;
	struct sdp_op_desc sdp_op;
	struct pdp_op_desc pdp_op;
	struct cdp_op_desc cdp_op;
	struct rubik_op_desc rubik_op;
};

#endif