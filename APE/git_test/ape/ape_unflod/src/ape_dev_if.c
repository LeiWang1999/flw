#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "ape_dev_if.h"
#include "log.h"
int ape_fd = -1;
unsigned int * ape_ptr;
unsigned int * ram_ptr;

#define APE_REG_ADDR 0x10200000
#define APE_REG_SIZE 0x20000

#define APE_RAM_ADDR  0xc0000000
#define APE_RAM_SIZE 0x30000000

#define ARM_APE 0

ape_error init_ape_dev() {
    #if ARM_APE
    int ape_fd = -1;

    ape_fd = open("/dev/mem",O_RDWR|O_SYNC);
   
    ape_ptr = (unsigned int *)mmap(NULL,APE_REG_SIZE,PROT_READ|PROT_WRITE, MAP_SHARED, ape_fd, APE_REG_ADDR);
    ram_ptr = (unsigned int *)mmap(NULL,APE_RAM_SIZE,PROT_READ|PROT_WRITE, MAP_SHARED, ape_fd, APE_RAM_ADDR);
    
    printf("ape_ptr- %p,ram_ptr %p\n",ape_ptr,ram_ptr);
    memset(ram_ptr,0xFFF,0xf00000);
    #endif
    return 0;
}

   
ape_error reg_write(unsigned int reg_num,unsigned int val) {
    #if ARM_APE
    
    volatile unsigned int* logical_address = (volatile unsigned int*)(ape_ptr + reg_num/4);
	*logical_address = val;
    LOG_DEBUG("reg_info addr: 0x%x addr:0x%x \n", reg_num,val);
    #endif
}


volatile unsigned int reg_read(volatile unsigned int reg_num)
{
    #if ARM_APE
	volatile unsigned int* logical_address = (volatile unsigned int*)(ape_ptr + reg_num/4);
	volatile unsigned int val = *logical_address;

	return val;
    #endif
}