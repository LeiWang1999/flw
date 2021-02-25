#include "ape_cmd.h"
#include "ape_error.h"
#include "ape_config.h"
#include "ape_interface.h"
#include "ape_dev_if.h"
#if JPEG
#include "picture_to_data.h"
#endif

#if RM_JPEG
#include "ape_types.h"
#endif

#if JPEG
#include "ape_type.h"
#endif

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
extern const struct ape_cmd cmd_data[APE_OPERATION_NUM];
void main()
{
#if JPEG
    int idx = 0 ; 
    struct Metadata in_data;

	ScU8 *input_data = (ScU8 *)malloc(INPUT_SIZE);
	memset(input_data , 0, INPUT_SIZE);

	picture_to_data("mnist_train_9.jpg",in_data,input_data);
#endif 

    x_ape_init((void *)cmd_data,APE_OPERATION_NUM);

#if JPEG
    x_ape_forward((void *)cmd_data, input_data, APE_OPERATION_NUM);
#endif
    
#if RM_JPEG
    x_ape_forward((void *)cmd_data, APE_OPERATION_NUM);
#endif 

    return ;
}

#endif 