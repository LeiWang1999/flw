#ifndef _APE_INTERFACE_H
#define _APE_INTERFACE_H

#include "ape_error.h"
#include "stdint.h"

#define APE_OPERATION_NUM 40
#define PRECISION_INT8		0
#define PRECISION_INT16		1
#define PRECISION_FP16		2

#define DESTINATION_DMA		1

#define WEIGHT_FORMAT_COMPRESSED	1

#define MEAN_FORMAT_DISABLE     0
#define MEAN_FORMAT_ENABLE      1

#define DESTINATION_PROCESSOR	0
#define DESTINATION_DMA		1



ape_error x_ape_init(void * cmd_data,int cmd_size);

ape_error x_ape_forward(void * cmd_data,int cmd_size);

#endif
