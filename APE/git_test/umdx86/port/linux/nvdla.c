/*
 * Copyright (c) 2017-2018, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define _GNU_SOURCE

#include <dlaerror.h>
#include <dlatypes.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <drm.h>
#include <drm_mode.h>
#include <stdlib.h>

#include "nvdla.h"
#include "nvdla_inf.h"
#include "nvdla_ioctl.h"
#include "nvdla_os_inf.h"

#define NVDLA_DEVICE_NODE "/dev/dri/renderD128"

#define NVDLA_MEM_READ (PROT_READ)
#define NVDLA_MEM_WRITE (PROT_WRITE)


NvDlaError VirtKmdAlloc(void *mem_info)
{
	struct nvdla_gem_create_args *alloc_info = (struct nvdla_gem_create_args *)mem_info;
    printf(" VirtKmdAlloc  pointer %p,mem_info %p, %d, %d, %d\n",alloc_info,mem_info,alloc_info->size,alloc_info->flags,alloc_info->buf_ptr);
	if(alloc_info==NULL) {
		printf(" VirtKmdAlloc input mem_info is NULL \n");
		return NvDlaError_BadValue;
	}
	if(alloc_info->size==0) {
        printf(" VirtKmdAlloc mem_info size is 0 \n");
		return NvDlaError_BadValue;
	}
	
	alloc_info->buf_ptr = aligned_alloc(64,alloc_info->size);

    printf(" VirtKmdAlloc %p\n",alloc_info->buf_ptr);
	if(alloc_info->buf_ptr == NULL) {
        printf(" VirtKmdAlloc failed size is %d \n",alloc_info->size);
		return NvDlaError_BadValue;
	}
	return NvDlaSuccess;
}

/*
	void* mem_info is pointer to struct ape_gem_destroy_args
*/
NvDlaError VirtKmdFree(void *mem_info)
{
	struct nvdla_gem_destroy_args *alloc_info = (struct nvdla_gem_destroy_args *)mem_info;
	if(alloc_info==NULL || alloc_info->buf_ptr == 0) {
		return NvDlaError_BadValue;
	}
    printf(" VirtKmdFree  pointer %p,mem_info %p, %d, %d, %p\n",alloc_info,mem_info,alloc_info->size,alloc_info->flags,alloc_info->buf_ptr);


	free(alloc_info->buf_ptr);
	return NvDlaSuccess;
}



NvDlaError
NvDlaAllocMem(void *session_handle, void *device_handle, void **mem_handle,
                void **pData, NvU32 size, NvDlaHeap heap)
{
    int err = 0;
    NvDlaMemHandle hMem;
    struct nvdla_gem_create_args create_args;
    struct nvdla_gem_map_offset_args map_args;
    NvDlaDeviceHandle hDlaDev = (NvDlaDeviceHandle)device_handle;

    
    hMem = (NvDlaMemHandle)malloc(sizeof(struct NvDlaMemHandleRec));
    *mem_handle = hMem;

    memset(*mem_handle, 0, sizeof(struct NvDlaMemHandleRec));

    memset(&create_args, 0, sizeof(create_args));

    create_args.size = size;
    printf(" enter alloc size=%d\n", size);
    err = VirtKmdAlloc(&create_args);
    if (err) {
        printf("Failed to allocate handle err=%d errno=%d\n", err, errno);
        err = -errno;
        goto free_mem_handle;
    }

    *pData = (void*)create_args.buf_ptr;
    printf(" alloc pData = %p\n", *pData);
    hMem->buf_ptr = create_args.buf_ptr;

    printf(" exit alloc\n");
    return 0;

free_mem_handle:
    free(hMem);
    *mem_handle = NULL;
    return err;
}

NvDlaError
NvDlaFreeMem(void *session_handle, void *device_handle, void *mem_handle, void *pData, NvU32 size)
{
    int err;
    struct nvdla_gem_destroy_args args;
    NvDlaMemHandle hMem = (NvDlaMemHandle)mem_handle;
    NvDlaDeviceHandle hDlaDev = (NvDlaDeviceHandle)device_handle;

    printf("enter  free\n");
    if (hMem == 0)
        return NvDlaError_BadParameter;

    args.buf_ptr = hMem->buf_ptr;
    
    err = VirtKmdFree(&args);
    if (err) {
        printf("Failed to destroy handle err=%d errno=%d\n", err, errno);
        return NvDlaError_IoctlFailed;
    }
    free(hMem);
    printf("exit  free\n");
    return NvDlaSuccess;
}



/*NvDlaError
NvDlaSubmit(void *session_handle, void *device_handle, NvDlaTask *pTasks, NvU32 num_tasks)
{
   
    struct nvdla_mem_handle address_list[num_tasks][NVDLA_MAX_BUFFERS_PER_TASK];
    struct nvdla_ioctl_submit_task tasks[num_tasks];
    
    uint32_t i;
    int err;
    
    printf("enter\n");
    struct nvdla_submit_args args;


    memset(&args, 0, sizeof(args));
    args.tasks = (uintptr_t)tasks;
    args.num_tasks = num_tasks;

    for (i = 0; i < num_tasks; i++) {
        uint32_t num_addresses = tasks[i].num_addresses =
                            pTasks[i].num_addresses;
        uint32_t j;

        tasks[i].address_list = (uintptr_t)address_list[i];
        for (j = 0; j < num_addresses; j++) {
            NvDlaMemHandle mem_handle = (NvDlaMemHandle)pTasks[i].address_list[j].handle;

            address_list[i][j].buf_ptr = mem_handle->buf_ptr;
            address_list[i][j].offset = pTasks[i].address_list[j].offset;
        }
    }

    err = APEKmdSubmit(&args);
    if(err < 0){
        printf("%s: Error IOCTL failed (%s)\n", __func__, strerror(errno));
        return NvDlaSuccess;
    }

    printf("exit\n");
    return NvDlaSuccess;

}*/

NvDlaError
NvDlaInitialize(void **session_handle)
{
    if(!session_handle)
        return NvDlaError_BadParameter;
    *session_handle = NULL;

    return NvDlaSuccess;
}

void
NvDlaDestroy(void *session_handle)
{
    (void)session_handle;

    return;
}




NvDlaError
NvDlaOpen(void *session_handle, NvU32 instance, void **device_handle)
{
    NvDlaContext *pContext = NULL;
    NvDlaError e = NvDlaSuccess;
    printf("enter  open\n");
    if (instance > 0)
        return NvDlaError_BadParameter;

    if (!device_handle)
        return NvDlaError_BadParameter;

    pContext = (NvDlaContext*)malloc(sizeof(NvDlaContext));
    printf("pContex %p\n",pContext);
    if (!pContext) {
        return NvDlaError_InsufficientMemory;
    }

    memset(pContext, 0, sizeof(NvDlaContext));

    *device_handle = (void *)pContext;

    printf("exit  open \n");
    return NvDlaSuccess;
}




void
NvDlaClose(void *hDlaDevice)
{
    NvDlaDeviceHandle device_handle = (NvDlaDeviceHandle)hDlaDevice;

    if (hDlaDevice == NULL)
        return;

    NvDlaFree(device_handle);
    return;
}





