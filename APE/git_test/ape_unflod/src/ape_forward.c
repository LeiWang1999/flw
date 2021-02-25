#include "ape_interface.h"
#include "ape_cmd.h"
#include "log.h"
#include "ape_dev_if.h"
#include "ape_config.h"
#include "engine_data.h"
#include "ape_engine_internal.h"


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

    ape_wait_isr[cmd_data->op_type]((void *)cmd_data);

exit:
    return e;
}

#if JPEG
ape_error x_ape_forward(void * cmd_data,void *input_data, int cmd_size) {
#endif
#if RM_JPEG
ape_error x_ape_forward(void * cmd_data, int cmd_size) {
#endif

    int ape_idx = 0;
    struct ape_cmd *ape_cmd_queue;
    ape_error e = ape_success;
    ape_cmd_queue = (struct ape_cmd * )cmd_data;

#if JPEG
    mem_ptr[3] = dram_init(INPUT_SIZE,3);
    dram_copy(INPUT_SIZE,input_data, 3);
#endif

    for(ape_idx = 0; ape_idx < cmd_size ; ape_idx++) {
        
        e = process_cmd(&ape_cmd_queue[ape_idx]);
        if(e != ape_success) {
            goto exit;
        }
        printf("ape_idx %d\n",ape_idx);
        wait_isr(&ape_cmd_queue[ape_idx]);
        if(e != ape_success) {
            goto exit;
        }
    }
    /*
        Output in index 2 offset 16384
        Different loadable output with different index offset
    */
    get_dram_context(2,OUT_OFFSET);

exit:
    return e;
}