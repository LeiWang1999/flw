#ifndef _ENGINE_DATA_H
#define _ENGINE_DATA_H
#include "ape_cmd.h"
#include "ape_types.h"
extern void (*ape_fun[CMD_OP_NUM])(void *);
extern void (*ape_parse[APE_OP_NUM])(void *);

void * x_ape_get_surface_data(void) ;
void * x_ape_get_op_data(void) ;
void * x_ape_get_dma_address_data(void);
#endif