#ifndef _APE_DEV_INTERFACE_H
#define _APE_DEV_INTERFACE_H

#include "ape_error.h"

ape_error init_ape_dev();

ape_error reg_write(unsigned int reg,unsigned int val);

volatile unsigned int reg_read(volatile unsigned int reg_num);

#endif