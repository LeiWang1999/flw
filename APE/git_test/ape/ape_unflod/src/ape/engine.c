#include "ape_debug.h"
#include "ape_error.h"
#include "ape_interface.h"
#include "ape_cmd.h"
#include "ape_engine_internal.h"
#include "common.h"
#include "engine_data.h"
#include "log.h"


//struct ape_roi_desc roi_desc;

uint64_t * mem_ptr[3];

/**
 * Get DMA data cube address
 */
int32_t
dla_get_dma_cube_address(void *driver_context, void *task_data,
                    int16_t index, 
                    uint32_t offset, void *dst_ptr,
					uint32_t destination)
{
	int32_t ret = 0;
	uint64_t *pdst = (uint64_t *)dst_ptr;

    dst_ptr =  mem_ptr[index];

	pdst[0] += offset;
	//LOG_DEBUG("mem_ptr[%d] :%p offset=%d \n",index, dst_ptr,offset);
	
exit:
	return ret;
}


int
dla_read_input_address(struct data_cube *data,
		       uint64_t *address)   
		       //uint8_t roi_index,
		      // uint8_t bpp)
{
	//uint64_t roi_desc_addr;
	int32_t ret = not_initialized;
	//struct dla_engine *en = dla_get_engine();
  
	/**
	 * If memory type is HW then no address required
	 */
	if (data->type == DLA_MEM_HW) {
		ret = 0;
		goto exit;
	}

	/**
	 * If address list index is not -1 means this address has to
	 * be read from address list
	 */
	if (data->address != -1) {

		/**
		 * But if other parameters indicate that this is input layer
		 * for dynamic ROI then it is an error
		 */
		/*if (en->network->dynamic_roi &&
			en->network->input_layer == op_addr)
			goto exit;*/
		ret = dla_get_dma_cube_address(NULL,NULL,
						data->address,
						data->offset,
						(void *)address,
						DESTINATION_DMA);
		goto exit;
	}
#if 0
	/**
	 * Check if it is dynamic ROI and this is input layer
	 */
	/*if (pdp_data->dynamic_roi && pdp_data->input_layer == op_addr) {
		if (!pdp_data->surface_index)
			goto exit;*/

		/* Calculate address of ROI descriptor in array */
		roi_desc_addr = pdp_data->roi_array_addr;

		/* Read ROI descriptor */
		ret = dla_data_read(pdp_data,roi_desc_addr,
				(void *)&roi_desc,sizeof(roi_desc),
				sizeof(struct ape_roi_array_desc) +
				roi_index * sizeof(struct ape_roi_desc));
		if (ret)
			goto exit;

		/* Calculate ROI address */
		*address = en->task->surface_addr;
		*address += (roi_desc.top * data->line_stride) +
						(bpp * roi_desc.left);
      }
#endif

exit:
	return;
}