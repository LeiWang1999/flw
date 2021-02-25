#include "ape_types.h"
#include "log.h"

static void
dla_debug_data_cube(struct data_cube *cube)
{
	LOG_DEBUG("    type          = %u\n", cube->type);
	LOG_DEBUG("    address       = %d\n", cube->address);
	LOG_DEBUG("    width         = %d\n", cube->width);
	LOG_DEBUG("    height        = %d\n", cube->height);
	LOG_DEBUG("    channel       = %d\n", cube->channel);
	LOG_DEBUG("    size          = %u\n", cube->size);
	LOG_DEBUG("    line_stride   = %u\n", cube->line_stride);
	LOG_DEBUG("    surf_stride   = %u\n", cube->surf_stride);
	LOG_DEBUG("    plane_stride  = %u\n", cube->plane_stride);
	LOG_DEBUG("]\n");
}

static void
dla_debug_converter(struct cvt_param *cvt)
{
	LOG_DEBUG("[ scale = %d, truncate = %u, enable = %u, offset = %d ]\n",
			cvt->scale, cvt->truncate, cvt->enable, cvt->offset);
}

void
dla_debug_conv_surface_desc(struct conv_surface_desc *desc, int32_t roi)
{
	LOG_DEBUG("*********************************************************\n");
	//LOG_DEBUG("APE FW ROI[%d]: dla_conv_surface_desc\n", roi);
	LOG_DEBUG("---------------------------------------------------------\n");
	LOG_DEBUG("weight_data         = [ data_cube =>\n");
	dla_debug_data_cube(&desc->weight_data);
	LOG_DEBUG("wmb_data            = [ data_cube =>\n");
	dla_debug_data_cube(&desc->wmb_data);
	LOG_DEBUG("wgs_data            = [ data_cube =>\n");
	dla_debug_data_cube(&desc->wgs_data);
	LOG_DEBUG("src_data            = [ data_cube =>\n");
	dla_debug_data_cube(&desc->src_data);
	LOG_DEBUG("dst_data            = [ data_cube =>\n");
	dla_debug_data_cube(&desc->dst_data);
	LOG_DEBUG("offset_u            = %ld\n", desc->offset_u);
	LOG_DEBUG("in_line_uv_stride   = %u\n", desc->in_line_uv_stride);
}

void
dla_debug_sdp_surface_desc(struct sdp_surface_desc *desc, int32_t roi)
{
	LOG_DEBUG("*********************************************************\n");
	LOG_DEBUG("APE FW ROI[%d]: dla_sdp_surface_desc\n", roi);
	LOG_DEBUG("---------------------------------------------------------\n");
	LOG_DEBUG("src_data            = [ dla_data_cube =>\n");
	dla_debug_data_cube(&desc->src_data);
	LOG_DEBUG("x1_data             = [ dla_data_cube =>\n");
	dla_debug_data_cube(&desc->x1_data);
	LOG_DEBUG("x2_data             = [ dla_data_cube =>\n");
	dla_debug_data_cube(&desc->x2_data);
	LOG_DEBUG("y_data              = [ dla_data_cube =>\n");
	dla_debug_data_cube(&desc->y_data);
	LOG_DEBUG("dst_data            = [ dla_data_cube =>\n");
	dla_debug_data_cube(&desc->dst_data);
}

void
dla_debug_pdp_surface_desc(struct pdp_surface_desc *desc, int32_t roi)
{
	LOG_DEBUG("*********************************************************\n");
	LOG_DEBUG("APE FW ROI[%d]: dla_pdp_surface_desc\n", roi);
	LOG_DEBUG("---------------------------------------------------------\n");
	LOG_DEBUG("src_data            = [ dla_data_cube =>\n");
	dla_debug_data_cube(&desc->src_data);
	LOG_DEBUG("dst_data            = [ dla_data_cube =>\n");
	dla_debug_data_cube(&desc->dst_data);
}

void
dla_debug_cdp_surface_desc(struct cdp_surface_desc *desc, int32_t roi)
{
	LOG_DEBUG("*********************************************************\n");
	LOG_DEBUG("NVDLA FW ROI[%d]: dla_cdp_surface_desc\n", roi);
	LOG_DEBUG("---------------------------------------------------------\n");
	LOG_DEBUG("src_data            = [ dla_data_cube =>\n");
	LOG_DEBUG(&desc->src_data);
	LOG_DEBUG("dst_data            = [ dla_data_cube =>\n");
	LOG_DEBUG(&desc->dst_data);
}

void
dla_debug_conv_op_desc(struct conv_op_desc *desc, int32_t roi)
{
	LOG_DEBUG("*********************************************************\n");
	LOG_DEBUG("NVDLA FW ROI[%d]: dla_conv_op_desc\n", roi);
	LOG_DEBUG("---------------------------------------------------------\n");
	LOG_DEBUG("conv_mode          = %u\n", desc->conv_mode);
	LOG_DEBUG("data_reuse         = %u\n", desc->data_reuse);
	LOG_DEBUG("weight_reuse       = %u\n", desc->weight_reuse);
	LOG_DEBUG("skip_data_rls      = %u\n", desc->skip_data_rls);
	LOG_DEBUG("skip_weight_rls    = %u\n", desc->skip_weight_rls);
	LOG_DEBUG("entry_per_slice    = %u\n", desc->entry_per_slice);
	LOG_DEBUG("data_format        = %u\n", desc->data_format);
	LOG_DEBUG("pixel_mapping      = %u\n", desc->pixel_mapping);
	LOG_DEBUG("fetch_grain        = %u\n", desc->fetch_grain);
	LOG_DEBUG("batch              = %u\n", desc->batch);
	LOG_DEBUG("weight_format      = %u\n", desc->weight_format);
	LOG_DEBUG("data_bank          = %u\n", desc->data_bank);
	LOG_DEBUG("weight_bank        = %u\n", desc->weight_bank);
	LOG_DEBUG("batch_stride       = %u\n", desc->batch_stride);
	LOG_DEBUG("post_extension     = %u\n", desc->post_extension);
	LOG_DEBUG("pixel_override     = %u\n", desc->pixel_override);
	LOG_DEBUG("release            = %u\n", desc->release);
	LOG_DEBUG("input_width_csc    = %u\n", desc->input_width_csc);
	LOG_DEBUG("input_height_csc   = %u\n", desc->input_height_csc);
	LOG_DEBUG("input_channel_csc  = %u\n", desc->input_channel_csc);
	LOG_DEBUG("kernel_width_csc   = %u\n", desc->kernel_width_csc);
	LOG_DEBUG("kernel_height_csc  = %u\n", desc->kernel_height_csc);
	LOG_DEBUG("kernel_channel_csc = %u\n", desc->kernel_channel_csc);
	LOG_DEBUG("input_width_cmac   = %u\n", desc->input_width_cmac);
	LOG_DEBUG("input_height_cmac  = %u\n", desc->input_height_cmac);
	LOG_DEBUG("bytes_per_kernel   = %u\n", desc->bytes_per_kernel);
	LOG_DEBUG("mean_ry            = %d\n", desc->mean_ry);
	LOG_DEBUG("mean_gu            = %d\n", desc->mean_gu);
	LOG_DEBUG("mean_bv            = %d\n", desc->mean_bv);
	LOG_DEBUG("mean_ax            = %d\n", desc->mean_ax);
	LOG_DEBUG("mean_format        = %u\n", desc->mean_format);
	LOG_DEBUG("conv_stride_x      = %u\n", desc->conv_stride_x);
	LOG_DEBUG("conv_stride_y      = %u\n", desc->conv_stride_y);
	LOG_DEBUG("pad_x_left         = %u\n", desc->pad_x_left);
	LOG_DEBUG("pad_x_right        = %u\n", desc->pad_x_right);
	LOG_DEBUG("pad_y_top          = %u\n", desc->pad_y_top);
	LOG_DEBUG("pad_y_bottom       = %u\n", desc->pad_y_bottom);
	LOG_DEBUG("dilation_x         = %u\n", desc->dilation_x);
	LOG_DEBUG("dilation_y         = %u\n", desc->dilation_y);
	LOG_DEBUG("pra_truncate       = %u\n", desc->pra_truncate);
	LOG_DEBUG("in_precision       = %u\n", desc->in_precision);
	LOG_DEBUG("out_precision      = %u\n", desc->out_precision);
	LOG_DEBUG("pad_val            = %d\n", desc->pad_val);
	LOG_DEBUG("in_cvt             =\n");
	dla_debug_converter(&desc->in_cvt);
	LOG_DEBUG("out_cvt            =\n");
	dla_debug_converter(&desc->out_cvt);
}


void
dla_debug_pdp_op_desc(struct pdp_op_desc *desc, int32_t roi)
{
	int32_t i;

	LOG_DEBUG("*********************************************************\n");
	LOG_DEBUG("APE FW ROI[%d]: pdp_op_desc\n", roi);
	LOG_DEBUG("---------------------------------------------------------\n");
	LOG_DEBUG("precision               = %u\n", desc->precision);
	LOG_DEBUG("padding_value           = [\n");
	for (i = 0; i < PDP_PAD_VAL_NUM; i++)
		LOG_DEBUG(" %d\n", desc->padding_value[i]);
	LOG_DEBUG("]\n");
	LOG_DEBUG("split_num               = %u\n", desc->split_num);
	LOG_DEBUG("partial_in_width_first  = %u\n",
					desc->partial_in_width_first);
	LOG_DEBUG("partial_in_width_mid    = %u\n", desc->partial_in_width_mid);
	LOG_DEBUG("partial_in_width_last   = %u\n", desc->partial_in_width_last);
	LOG_DEBUG("partial_width_first     = %u\n", desc->partial_width_first);
	LOG_DEBUG("partial_width_mid       = %u\n", desc->partial_width_mid);
	LOG_DEBUG("partial_width_last      = %u\n", desc->partial_width_last);
	LOG_DEBUG("pool_mode               = %u\n", desc->pool_mode);
	LOG_DEBUG("pool_width              = %u\n", desc->pool_width);
	LOG_DEBUG("pool_height             = %u\n", desc->pool_height);
	LOG_DEBUG("stride_x                = %u\n", desc->stride_x);
	LOG_DEBUG("stride_y                = %u\n", desc->stride_y);
	LOG_DEBUG("pad_left                = %u\n", desc->pad_left);
	LOG_DEBUG("pad_right               = %u\n", desc->pad_right);
	LOG_DEBUG("pad_top                 = %u\n", desc->pad_top);
	LOG_DEBUG("pad_bottom              = %u\n", desc->pad_bottom);
}

static void
dla_debug_sdp_op(struct sdp_op *sdp_op)
{
	LOG_DEBUG("    enable         = %u\n", sdp_op->enable);
	LOG_DEBUG("    alu_type       = %u\n", sdp_op->alu_type);
	LOG_DEBUG("    type           = %u\n", sdp_op->type);
	LOG_DEBUG("    mode           = %u\n", sdp_op->mode);
	LOG_DEBUG("    act            = %u\n", sdp_op->act);
	LOG_DEBUG("    shift_value    = %u\n", sdp_op->shift_value);
	LOG_DEBUG("    truncate       = %u\n", sdp_op->truncate);
	LOG_DEBUG("    precision      = %u\n", sdp_op->precision);
	LOG_DEBUG("    alu_operand    = %d\n", sdp_op->alu_operand);
	LOG_DEBUG("    mul_operand    = %d\n", sdp_op->mul_operand);
	LOG_DEBUG("cvt.alu_cvt          =\n");
	dla_debug_converter(&sdp_op->cvt.alu_cvt);
	LOG_DEBUG("cvt.mul_cvt          =\n");
	dla_debug_converter(&sdp_op->cvt.mul_cvt);
	LOG_DEBUG("]\n");
}

void
dla_debug_sdp_op_desc(struct sdp_op_desc *desc, int32_t roi)
{
	LOG_DEBUG("*********************************************************\n");
	LOG_DEBUG("NVDLA FW ROI[%d]: dla_sdp_op_desc\n", roi);
	LOG_DEBUG("---------------------------------------------------------\n");
	LOG_DEBUG("src_precision    = %u\n", desc->src_precision);
	LOG_DEBUG("dst_precision    = %u\n", desc->dst_precision);
	LOG_DEBUG("lut_index        = %d\n", desc->lut_index);
	LOG_DEBUG("out_cvt          =\n");
	dla_debug_converter(&desc->out_cvt);
	LOG_DEBUG("conv_mode        = %u\n", desc->conv_mode);
	LOG_DEBUG("batch_num        = %u\n", desc->batch_num);
	LOG_DEBUG("batch_stride     = %u\n", desc->batch_stride);
	LOG_DEBUG("x1_op            = [ dla_sdp_op =>\n");
	dla_debug_sdp_op(&desc->x1_op);
	LOG_DEBUG("x2_op            = [ dla_sdp_op =>\n");
	dla_debug_sdp_op(&desc->x2_op);
	LOG_DEBUG("y_op             = [ dla_sdp_op =>\n");
	dla_debug_sdp_op(&desc->y_op);
}

void
dla_debug_cdp_op_desc(struct cdp_op_desc *desc, int32_t roi)
{
	LOG_DEBUG("*********************************************************\n");
	LOG_DEBUG("NVDLA FW ROI[%d]: dla_cdp_op_desc\n", roi);
	LOG_DEBUG("---------------------------------------------------------\n");
	LOG_DEBUG("in_precision      = %u\n", desc->in_precision);
	LOG_DEBUG("out_precision     = %u\n", desc->out_precision);
	LOG_DEBUG("lut_index         = %d\n", desc->lut_index);
	LOG_DEBUG("in_cvt             =\n");
	dla_debug_converter(&desc->in_cvt);
	LOG_DEBUG("out_cvt             =\n");
	dla_debug_converter(&desc->out_cvt);
	LOG_DEBUG("local_size        = %u\n", desc->local_size);
	LOG_DEBUG("bypass_sqsum      = %u\n", desc->bypass_sqsum);
	LOG_DEBUG("bypass_out_mul    = %u\n", desc->bypass_out_mul);
}