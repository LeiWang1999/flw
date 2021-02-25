#include "ape_cmd.h"
#include "ape_error.h"
#include "ape_interface.h"

#define TEST_RANDOM 0


#if TEST_RANDOM
#include <stdlib.h>
#include <time.h>
struct ape_cmd cmd_data[APE_OPERATION_NUM]= {};

void main()
{
    int idx = 0 ;
    srand( (unsigned)time( NULL ) );
    for(idx = 0 ; idx <APE_OPERATION_NUM ; idx++) {
        cmd_data[idx].cmd_type = rand() %CMD_NUM;
        cmd_data[idx].op_type = rand() % APE_OP_NUM;
        cmd_data[idx].wait_cnt = rand() % 5;
    }
    x_ape_init((void *)cmd_data,APE_OPERATION_NUM);

    x_ape_forward((void *)cmd_data,APE_OPERATION_NUM);

    return ;
}
#else

struct ape_cmd cmd_data[APE_OPERATION_NUM] = {
                               {SETHWIDX_OP,APE_OP_CONV,0,0,0,0},{PROGRAM_OP,APE_OP_CONV,0,0,0,0},
                               {SETHWIDX_OP,APE_OP_CONV,0,0,0,3},{PROGRAM_OP,APE_OP_CONV,0,0,0,3},
                               {SETHWIDX_OP,APE_OP_SDP, 0,0,0,1},{PROGRAM_OP,APE_OP_SDP, 0,0,0,1},
                               {SETHWIDX_OP,APE_OP_SDP, 0,0,0,1},{ENABLE_OP,APE_OP_SDP, 0,0,0,1},
                               {SETHWIDX_OP,APE_OP_CONV,0,0,0,0},{ENABLE_OP,APE_OP_CONV,0,0,0,0},
                               {SETHWIDX_OP,APE_OP_SDP, 1,0,0,4},{PROGRAM_OP,APE_OP_SDP, 1,0,0,4},
                               {SETHWIDX_OP,APE_OP_SDP, 1,0,0,4},{ENABLE_OP,APE_OP_SDP, 1,0,0,4},
                               {SETHWIDX_OP,APE_OP_PDP, 0,0,0,2} , {PROGRAM_OP,APE_OP_PDP,0,0,0,2},
                               {SETHWIDX_OP,APE_OP_PDP, 1,0,0,5} , {PROGRAM_OP,APE_OP_PDP,1,0,2,5},
                               {SETHWIDX_OP,APE_OP_CONV,0,0,0,6},{PROGRAM_OP,APE_OP_CONV,0,0,0,6},
                               {SETHWIDX_OP,APE_OP_SDP, 0,0,0,2},{ENABLE_OP,APE_OP_SDP, 0,0,0,2},
                               {SETHWIDX_OP,APE_OP_SDP, 0,0,0,7},{PROGRAM_OP,APE_OP_SDP, 0,0,0,7},
                               {SETHWIDX_OP,APE_OP_SDP, 0,0,0,7},{ENABLE_OP,APE_OP_SDP, 0,0,0,7},
                               {SETHWIDX_OP,APE_OP_CONV, 1,0,1,3},{ENABLE_OP,APE_OP_CONV, 1,0,0,3},
                               {SETHWIDX_OP,APE_OP_CONV,1,0,2,8},{PROGRAM_OP,APE_OP_CONV,1,0,0,8},
                               {SETHWIDX_OP,APE_OP_PDP, 1,0,0,5} , {ENABLE_OP,APE_OP_PDP,1,0,0,5},
                               {SETHWIDX_OP,APE_OP_SDP, 1,0,0,9},{PROGRAM_OP,APE_OP_SDP, 1,0,0,9},
                               {SETHWIDX_OP,APE_OP_SDP, 1,0,0,9},{ENABLE_OP,APE_OP_SDP, 1,0,1,9},
                               {SETHWIDX_OP,APE_OP_CONV,0,0,0,6},{ENABLE_OP,APE_OP_CONV,0,0,2,6},
                               {SETHWIDX_OP,APE_OP_CONV,1,0,0,8},{ENABLE_OP,APE_OP_CONV,1,0,2,8},
                               };
void main()
{
    int idx = 0 ;
   
    x_ape_init((void *)cmd_data,40);

    x_ape_forward((void *)cmd_data,40);

    return ;
}

#endif 