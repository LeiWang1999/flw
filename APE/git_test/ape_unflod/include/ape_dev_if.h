#ifndef _APE_DEV_INTERFACE_H
#define _APE_DEV_INTERFACE_H

#define ARM_APE 0
#define RM_JPEG 1
#define JPEG 0
#define DUMP_DATA 0

#include "ape_error.h"

#if RM_JPEG
#include "ape_types.h"
#endif

#if JPEG
#include "ape_type.h"
#include "ape_types.h"
#endif

ape_error init_ape_dev();

ape_error reg_write(ScU32 reg,ScU32 val);

volatile unsigned int reg_read(volatile unsigned int reg_num);

volatile void poll_field_not_equal(volatile unsigned int reg_num, volatile unsigned int field, volatile unsigned int expect_value);

ScU32 dram_init(ScSU64 size,ScS32 index);

void dram_copy(ScSU64 size,void * data_ptr,ScS32 index);

void get_dram_context(ScS32 ram_idx,ScSU64 offset);

#endif