#include <stdio.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <sys/mman.h>
#include<stdint.h>
#include<errno.h>
#include "ape_dev_if.h"
#include "ape_config.h"
#include "log.h"
#include "ape_debug.h"

int ape_fd = -1;
ScU32 * ape_ptr;
ScU8* ram_ptr;
//#define ARM_APE 0

void * ram_vir_addr[10] ={};
ape_error init_ape_dev() {
#if ARM_APE
    int ape_fd = -1;

    ape_fd = open("/dev/mem",O_RDWR|O_SYNC);
   
    ape_ptr = (ScU32 *)mmap(NULL,APE_REG_SIZE,PROT_READ|PROT_WRITE, MAP_SHARED, ape_fd, APE_REG_ADDR);
    ram_ptr = (ScU8 *)mmap(NULL,APE_RAM_SIZE,PROT_READ|PROT_WRITE, MAP_SHARED, ape_fd, APE_RAM_ADDR);
    
    LOG_DEBUG("ape_ptr- %p,ram_ptr %p\n",ape_ptr,ram_ptr);
    //memset((ScU8*)ram_ptr,0x0,APE_RAM_SIZE);
    close(ape_fd);
#endif
    return ;
}

   
ape_error reg_write(ScU32 reg_num,ScU32 val) {
#if ARM_APE
    
    volatile ScU32* logical_address = (volatile ScU32*)(ape_ptr + reg_num/4);
	*logical_address = val;
    //LOG_DEBUG("reg_info addr: 0x%x addr:0x%x \n", reg_num,val);
#else
   // printf("reg_info :  addr: 0x%x reg: 0x%x\n", reg_num,val);
#endif
    
}

volatile void poll_field_not_equal(volatile ScU32 reg_num, volatile ScU32 field, volatile ScU32 expect_value)
{
#if ARM_APE
    volatile ScU32 data;

	LOG_DEBUG("poll_reg_num=%x, field = %x, expect_value=%x\n", reg_num, field, expect_value);

    while (1){
        data = reg_read(reg_num);

		if ((data & field) != expect_value){
			break;
		}

    }
#endif
}

volatile ScU32 reg_read(volatile ScU32 reg_num)
{
#if ARM_APE
	volatile ScU32* logical_address = (volatile ScU32*)(ape_ptr + reg_num/4);
	volatile ScU32 val = *logical_address;

return val;
    #else
    return 0xffff;
#endif
}


ScU32 dram_init(ScSU64 size,ScS32 index) {
#if ARM_APE
    ScU32 start_dram  = 0;
    ScU8 * start_vir_ptr = 0;
    static ScSU64 size_start = 0;
    
    
    ScU32 addr = NULL;
    if(size %4 ) {
        LOG_DEBUG("dram_malloc size wrong %d\n",size);
        return NULL;
    }
    
    start_dram = APE_RAM_ADDR+size_start;
    
    start_vir_ptr = ram_ptr +size_start;

    //memset((ScU8 *)start_vir_ptr,0x0,size);
    //start_vir_ptr = ram_ptr +size_start/4;
    //LOG_DEBUG("start_dram addr %p  size %x\n data_ptr %p\n",start_dram,size,data_ptr);
    
    ram_vir_addr[index] = start_vir_ptr;
    addr = start_dram;
    size_start = size_start +size;
    return addr;    
#endif
}

void dram_copy(ScSU64 size,void * data_ptr,ScS32 index) {
#if ARM_APE    

    if(data_ptr != NULL) {
       memcpy((ScU8 *)ram_vir_addr[index],(ScU8 *)data_ptr,size);
        //memset((ScU8 *)start_vir_ptr,0x0,size);
    } else {
        
        //addr = start_dram;
       // memset((ScU8 *)start_vir_ptr,0x0,size);
       
    }
    return  ;
#endif
}


void get_dram_context(ScS32 ram_idx,ScSU64 offset) {
#if ARM_APE
    ScU8 * value = (ScU8 *)(ram_vir_addr[ram_idx]+offset);
    LOG_DEBUG("value %p %p\n",(ScU8 *)ram_vir_addr[ram_idx],value);
  
    for(int i = 0 ;i < OUT_NUM ;i++) {
        int8_t tmp = value[i];
        LOG_DEBUG("value %d\n",tmp);
    }
    return ;
#endif
}


