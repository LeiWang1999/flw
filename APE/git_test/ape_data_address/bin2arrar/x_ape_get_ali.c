#include "x_ape_get_ali.h"
#include "ape_dev_if.h"
#include "ape_config.h"
#include "ape_interface.h"
#include "ape_engine_internal.h"
#include "log.h"

unsigned long long mem_ptr[14]={};

void x_ape_get_init_data(){

	ScU8 * weight_ptr =(ScU8 *)(x_ape_get_ali1());

	mem_ptr[ 1 ] = dram_init(WEIGHT_SIZE, 1 ); 
	dram_copy( WEIGHT_SIZE ,weight_ptr, 1 ); 

	mem_ptr[ 2 ] = dram_init( 5120000 , 2 );
	mem_ptr[ 13 ] = dram_init( 4096 , 13 );


	mem_ptr[ 3 ] = dram_init(INPUT_SIZE, 3 ); 
	mem_ptr[ 5 ] = dram_init(INPUT_SIZE, 5 ); 
	mem_ptr[ 6 ] = dram_init(INPUT_SIZE, 6 ); 
	mem_ptr[ 7 ] = dram_init(INPUT_SIZE, 7 ); 
	mem_ptr[ 8 ] = dram_init(INPUT_SIZE, 8 ); 


	mem_ptr[ 4 ] = dram_init(OUTPUT_SIZE, 4 ); 

	return ;

}
