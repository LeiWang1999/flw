#ifndef _DUMP_DATA_H_
#define _DUMP_DATA_H_

#include <stdint.h>

//int32_t write_config_file(void * config_ptr,int32_t config_size);
int32_t dump_data(uint64_t phys_addr,uint32_t size,uint8_t op_type, uint32_t memid, uint32_t offset);

#endif