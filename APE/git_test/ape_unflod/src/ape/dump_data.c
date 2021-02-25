
#include "ape_dev_if.h"
#if DUMP_DATA
#include <sys/mman.h>
#include "dump_data.h"
#include<stdint.h>
#include<errno.h>
#include "log.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "ape_types.h"


int32_t dump_data(ScU64 phys_addr,ScU32 size,ScU8 op_type,ScU32 memid, ScU32 offset)
{
        
        FILE *file = NULL;
        char path[50];
        int w_size = 1024;
        void *reserve_virt_addr=NULL;
        
        int ape_fd = -1;
        ape_fd = open("/dev/mem",O_RDWR|O_SYNC);

        if(size==0 && phys_addr==0)
                LOG_DEBUG("error: unable to map the virtual address\n");     

        reserve_virt_addr=(void *)mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,ape_fd,phys_addr);

        LOG_DEBUG("reserve_virt_addr =0x%lx  phys_addr=0x%lx\n",(unsigned long) reserve_virt_addr,phys_addr);
        close(ape_fd);

        sprintf(path, "conv_op_type_%d_memid_%d_size_%d_offset_%d.bin",op_type,memid, size,offset);

        if(file == NULL)
               file = fopen(path, "w+");
        /*
        if (IS_ERR(file)) {
                LOG_DEBUG("error occured while opening file %s, exiting n", path);
                return 0;
        }*/
        LOG_DEBUG("(void *)reserve_virt_addr %p size  %d\n",reserve_virt_addr,size);
        
        if(size > 1024) {
                long reserve = size;
                
                for(int i = 0 ;i < size ; ) {
                        fwrite(reserve_virt_addr,1,w_size,file);
                        reserve  = reserve -1024;
                        LOG_DEBUG("(void *)reserve_virt_addr %p size  %d\n",reserve_virt_addr,size);
                        if(reserve < 1024) {
                              
                                w_size = reserve;
                        }else {
                                w_size = 1024;
                               
                        }
                        i = i+1024;
                        reserve_virt_addr += 1024;
                      
                }
        } else {
          if(fwrite(reserve_virt_addr,1,w_size,file)==0)
                LOG_DEBUG("printf打印标准输出：%s\n", strerror(errno));
        }
      
        fclose(file);

        return 0;
       
}

/*int32_t write_config_file(void * config_ptr,int32_t config_size){
#if 1
        loff_t pos;
        struct file *file = NULL;
        mm_segment_t old_fs;
        char path[50];
        FILE *fp;
        sprintf(path, "optype_%d_memid_%d_size_%lu_offset_%d.bin",optypr,memid, size,offset);
        
        //#define MY_FILE1 "/mnt/dump/conv_dump_weight_925696_remode.txt"

        printk("Hello, I'm the module that intends to write messages to file.\n");


        if(file == NULL)
               file = filp_open(path, O_RDWR | O_APPEND | O_CREAT, 0644);
        
        if (IS_ERR(file)) {
                printk("error occured while opening file %s, exiting n", MY_FILE1);
                return 0;
        }

        printk("(void *)conv_surface %p sizeof(struct dla_conv_surface_desc) %d\n",config_ptr,config_size);
        old_fs = get_fs();
    set_fs(KERNEL_DS);
    pos =0;
    vfs_write(file, (void *)config_ptr, config_size, &pos);

    filp_close(file,NULL);
#endif
   return 0;

}*/
#endif