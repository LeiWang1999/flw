#ifndef __APE_CMD_H_
#define __APE_CMD_H_

#include "ape_dev_if.h"
#include "ape_debug.h"

#define PROGRAM_OP  0
#define SETHWIDX_OP 1
#define ENABLE_OP   2
#define CMD_NUM 3


#define APE_OP_BDMA		0
#define APE_OP_CONV		1
#define APE_OP_SDP		2
#define APE_OP_PDP		3
#define APE_OP_CDP		4
#define APE_OP_RUBIK	5
#define APE_OP_NUM 6

#define CMD_OP_NUM 18 //CMD_NUM x APE_OP_NUM

struct ape_cmd {
	ScU8 cmd_type;/*bdma sdp conv pdp cdp rubik*/
	ScU8 op_type;/*perpare set_hw_idx enable*/
	
    ScU8 pingpong_id;/*hw_idx*/
	ScU8 rdma_need;
	
    ScU8 wait_cnt;/*operation compelted signal*/
    
	ScU8 data_idx;/*数据idx*/
	
    ScU8 reserved[2];
	
    void * surface_addr;
	
    void * op_addr;

    void (*process)(void *);
};

#endif