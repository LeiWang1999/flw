#ifndef _ENGINE_DATA_H
#define _ENGINE_DATA_H
#include "ape_cmd.h"
#include "ape_config.h"
#include "ape_dev_if.h"

extern ScU32 mem_ptr[5];
extern void (*ape_fun[CMD_OP_NUM])(void *);
extern void (*ape_parse[APE_OP_NUM])(void *);
void (*ape_wait_isr[APE_OP_NUM])(void *);

void * x_ape_get_surface_data(void) ;
void * x_ape_get_op_data(void) ;
void * x_ape_get_weight_data(void);

#if RM_JPEG
void * x_ape_get_input_data(void);
#endif
//void * x_ape_get_cmd_data(void);
extern const struct ape_cmd cmd_data[APE_OPERATION_NUM];
#endif