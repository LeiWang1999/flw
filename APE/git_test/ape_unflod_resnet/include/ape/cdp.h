#ifndef _CDP_H
#define _CDP_H
#include "ape_cmd.h"
void cdp_program(void * data);

void cdp_set_hw_idx(void * data);

void cdp_enable(void * data);

void cdp_parse(void *data);

void cdp_finish(void *data);
#endif 