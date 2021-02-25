#ifndef __APE_CONFIG_H_
#define __APE_CONFIG_H_

#define REG_SMALL

#define ATOM_SIZE 8

#define RESNET 1
#if RESNET
    #define APE_OPERATION_NUM 536
    //#define WEIGHT_SIZE 35602432
    #define WEIGHT_SIZE 925696
    #define ALLOC_SIZE  4071424
    #define INPUT_SIZE  401408

    #define OUT_OFFSET 2363392
    #define OUT_NUM    1000
#else
    #define APE_OPERATION_NUM 40
    #define WEIGHT_SIZE 925696
    #define ALLOC_SIZE  24576
    #define INPUT_SIZE  6272
    

    #define OUT_OFFSET 16384
    #define OUT_NUM 10
#endif

#define ZYNQ_BASE 0  //FPGA
#if ZYNQ_BASE 
    #define APE_REG_ADDR 0xa0000000
    #define APE_REG_SIZE 0x10000

    #define APE_RAM_ADDR  0x40000000
    #define APE_RAM_SIZE 0x40000000

#else
    #define APE_REG_ADDR 0x10200000
    #define APE_REG_SIZE 0x20000

    #define APE_RAM_ADDR  0xc0000000
    #define APE_RAM_SIZE 0x30000000
#endif

#ifdef REG_SMALL
    #include "ape_small.h"
#else
    #include "ape_initial.h"
#endif



#endif