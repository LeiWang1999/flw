#ifndef _DUMP_DATA_H_
#define _DUMP_DATA_H_
 

//int32_t write_config_file(void * config_ptr,int32_t config_size);
int32_t dump_data(unsigned long phys_addr,uint32_t size,uint32_t memid,uint16_t optype, uint32_t offset);

#endif