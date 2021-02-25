#include "ape_interface.h"
#include "ape_cmd.h"
#include "log.h"
ape_error process_cmd(struct ape_cmd *cmd_data) {
    ape_error e = ape_success;
    
    if(cmd_data->process == 0) {
        e = not_initialized;
        goto exit;
    }
    cmd_data->process((void *)cmd_data);
exit:
    return e;
}

ape_error wait_isr(struct ape_cmd * cmd_data) {
    ape_error e = ape_success;
    
    if(cmd_data->wait_cnt == 0) {
        goto exit;
    }
    do {
        //1.轮询中断状态
        //2.接收中断信号，两种方式
        LOG_DEBUG("wait_cnt %d\n",cmd_data->wait_cnt);
        cmd_data->wait_cnt--;
    } while (cmd_data->wait_cnt);
exit:
    return e;
}

ape_error x_ape_forward(void * cmd_data, int cmd_size) {
    int ape_idx = 0;
    struct ape_cmd *ape_cmd_queue;
    ape_error e = ape_success;
    ape_cmd_queue = (struct ape_cmd * )cmd_data;

    for(ape_idx = 0; ape_idx < cmd_size ; ape_idx++) {
        
        e = process_cmd(&ape_cmd_queue[ape_idx]);
        if(e != ape_success) {
            goto exit;
        }
        
        wait_isr(&ape_cmd_queue[ape_idx]);
        if(e != ape_success) {
            goto exit;
        }
    }
exit:
    return e;
}