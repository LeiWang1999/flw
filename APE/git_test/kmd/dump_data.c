#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <dla_interface.h>
#include <linux/mm.h>
#include <asm/io.h> /* for virt_to_phys */
#include "dump_data.h"
//#include <stdio.h>


int32_t dump_data(unsigned long phys_addr,uint32_t size, uint32_t memid,uint16_t optype, uint32_t offset)
{
        static char *reserve_virt_addr;
        loff_t pos;
        struct file *file = NULL;
        //mm_segment_t old_fs;
        char path[50];
        

        if(size==0 && phys_addr==0)
                dla_error("error: unable to map the virtual address\n");     
       
        reserve_virt_addr = ioremap( phys_addr ,size );
        dla_error("reserve_virt_addr =0x%lx  phys_addr=0x%lx\n",(unsigned long) reserve_virt_addr,phys_addr);
        // write_config_file(reserve_virt_addr,size);
        //*int32_t write_config_file(void * config_ptr,int32_t config_size){
        //#define MY_FILE1 "/mnt/dump/conv_dump_weight_925696_remode.txt"
        printk("Hello, I'm the module that intends to write messages to file.\n");
        sprintf(path, "op_type_%d_memid_%d_size_%d_offset_%d.bin",optype,memid, size,offset);

        if(file == NULL)
               file = filp_open(path, O_RDWR | O_APPEND | O_CREAT, 0644);
        
        if (IS_ERR(file)) {
                printk("error occured while opening file %s, exiting n", path);
                return 0;
        }
        printk("(void *)conv_surface %p sizeof(struct dla_conv_surface_desc) %d\n",reserve_virt_addr,size);
        // old_fs = get_fs();
        // set_fs(KERNEL_DS);
        pos =0;
        //kernel_write(file, (void *)reserve_virt_addr, size, &pos);
        kernel_write(file, (void *)reserve_virt_addr,size, pos );
        filp_close(file,NULL);

        return 0;
        
        //return reserve_virt_addr;
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