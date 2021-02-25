#ifndef __APE_CMD_H_
#define __APE_CMD_H_

#include <stdint.h>

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
	uint8_t cmd_type;/*bdma sdp conv pdp cdp rubik*/
	uint8_t op_type;/*perpare set_hw_idx enable*/
	
    uint8_t pingpong_id;/*hw_idx*/
	uint8_t rdma_need;
	
    uint8_t wait_cnt;/*operation compelted signal*/
    
	uint8_t data_idx;/*数据idx*/
	
    uint8_t reserved[2];
	
    void * surface_addr;
	
    void * op_addr;

    void (*process)(void *);
};

#endif