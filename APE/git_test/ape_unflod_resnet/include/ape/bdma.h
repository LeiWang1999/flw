#ifndef _BDMA_H
#define _BDMA_H
#include "ape_cmd.h"
void bdma_program(void * data);

void bdma_set_hw_idx(void * data);

void bdma_enable(void * data);

void bdma_parse(void *data);

void bdma_finish(void *data);
#endif 