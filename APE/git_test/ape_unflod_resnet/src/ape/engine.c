#include "ape_debug.h"
#include "ape_error.h"
#include "ape_interface.h"
#include "ape_cmd.h"
#include "ape_engine_internal.h"
#include "common.h"
#include "engine_data.h"
#include "log.h"


//struct ape_roi_desc roi_desc;

/**
 * Get DMA data cube address
 */
ScS32
ape_get_dma_cube_address(void *driver_context, void *task_data,
                    ScS16 index, 
                    ScU32 offset, void *dst_ptr,
					ScU32 destination)
{
	ScS32 ret = 0;
	ScU64 *pdst = (ScU64 *)dst_ptr;

    dst_ptr = (void *)mem_ptr[index];

	pdst[0] =mem_ptr[index]+ offset;
	LOG_DEBUG("mem_ptr[%d] %x dst_ptr :%x offset=%d \n",index,mem_ptr[index], pdst[0],offset);
	
exit:
	return ret;
}


ScS32
ape_read_input_address(struct data_cube *data,
		       ScU64 *address)   
		      
{
	//ScU64 roi_desc_addr;
	ScS32 ret = not_initialized;
	
  
	/**
	 * If memory type is HW then no address required
	 */
	if (data->type == APE_MEM_HW) {
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
		ret = ape_get_dma_cube_address(NULL,NULL,
						data->address,
						data->offset,
						(void *)address,
						DESTINATION_DMA);
		goto exit;
	}


exit:
	return 0;
}