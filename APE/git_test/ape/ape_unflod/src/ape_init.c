#include "ape_interface.h"
#include "ape_cmd.h"
#include "log.h"
#include "engine_data.h"
#include "ape_types.h"

int64_t surface_data[1000],op_data[1000];
void *mem_ptr[5]={};

ape_error init_opertation_weight_config(void* opeartion_queue,void *dat_ptr) {
    ape_error e = ape_success;
    return e;
}

ape_error init_opertation_io_address(void* opeartion_queue,void *dat_ptr) {
    ape_error e = ape_success;
    return e;
}

ape_error init_op_fun (struct ape_cmd * cmd_data) {
    /*得到一个二维指针*/
    //x_ape_fun_init
    ape_error e = ape_success;
    int get_fun_idx = cmd_data->op_type*CMD_NUM + cmd_data->cmd_type;

    if(get_fun_idx > CMD_OP_NUM) {
        e = bad_value;
        goto exit;
    }

    cmd_data->process = ape_fun[get_fun_idx];
    /*解析surface 中的id 获得真正地址*/
    if(cmd_data->cmd_type == PROGRAM_OP)
        ape_parse[cmd_data->op_type]((void *)cmd_data);
exit:
    return e;

}

ape_error init_cmd_queue(void * cmd_data,int cmd_size) {
    int ape_idx = 0;
    int data_idx = 0 ;
    ape_error e = ape_success;
    struct ape_cmd *ape_cmd_queue;

    //TODO 
    /*
        init_ape_operation_fun_array
    */
    void * output_ptr= (void *)malloc(16);
    uint8_t * surface_data = (uint8_t *)x_ape_get_surface_data();
    uint8_t * op_data = (uint8_t *)x_ape_get_op_data();
    uint8_t * weight_ptr =(uint8_t *)(x_ape_get_weight_data());
    uint8_t * input_ptr =(uint8_t *)(x_ape_get_input_data());

    mem_ptr[1] = weight_ptr;
    mem_ptr[3] = input_ptr;
    mem_ptr[4] = output_ptr;
    printf("mem_ptr[1] = %p \n",mem_ptr[1]);
    ape_cmd_queue = (struct ape_cmd * )cmd_data;
#if 0
    LOG_DEBUG("surface_container size %ld\n",sizeof(union surface_container));
    LOG_DEBUG("bdma_surface_desc size %ld\n",sizeof(struct bdma_surface_desc));
    LOG_DEBUG("conv_surface_desc size %ld\n",sizeof(struct conv_surface_desc));
    LOG_DEBUG("sdp_surface_desc size %ld\n",sizeof(struct sdp_surface_desc));
    LOG_DEBUG("pdp_surface_desc size %ld\n",sizeof(struct pdp_surface_desc));
    LOG_DEBUG("cdp_surface_desc size %ld\n",sizeof(struct cdp_surface_desc));
    LOG_DEBUG("rubik_surface_desc size %ld\n",sizeof(struct rubik_surface_desc));
#endif
LOG_DEBUG("operation_container size %ld\n",sizeof(union operation_container));
    for(ape_idx = 0 ; ape_idx < cmd_size ; ape_idx++) {
        data_idx = ape_cmd_queue[ape_idx].data_idx;
        
        ape_cmd_queue[ape_idx].surface_addr = surface_data +sizeof(union surface_container)*data_idx;//;
        LOG_DEBUG("data idx %d  ape_cmd_queue[%d].surface_addr %p \n",data_idx,ape_idx, (void *)ape_cmd_queue[ape_idx].surface_addr);
        ape_cmd_queue[ape_idx].op_addr = op_data +sizeof(union operation_container)*data_idx;
        LOG_DEBUG("data idx %d  ape_cmd_queue[%d].op_addr %p \n",data_idx,ape_idx, (void *)ape_cmd_queue[ape_idx].op_addr);
        init_op_fun(&ape_cmd_queue[ape_idx]);
    }
    return e;
}


ape_error x_ape_init(void * cmd_data,int cmd_size) {
    ape_error e = ape_success;

    init_cmd_queue(cmd_data,cmd_size);
exit:
    return e;
}