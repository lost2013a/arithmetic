#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>

#define    HELLO_MAJOR     232
#define    DEVICE_NAME     "HelloModule"
 
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h> 
 
//extern pid_t getpid(void);
 
static int hello_open(struct inode *inode, struct file *file){
    printk(KERN_ERR "hello open 2.\n");
    return 0;
}
 
static ssize_t hello_write(struct file *file, const char __user * buf, size_t count, loff_t *ppos){
	//char * p=buf;
	char p[5];
	
	copy_from_user(p, (char*)buf, 5);
	printk(KERN_ERR  "hello write 2.buf:%x count:%d, tid %d\n",buf,count,  current->pid);
	printk(KERN_ERR  "hello write 2.buf:%d,%d,%d,%d,%d\n",p[0],p[1],p[2],p[3],p[4]);
	p[1] = 20;
	p[2] = 50;
	
    return 0;
}
 
static struct file_operations hello_flops = {
    .owner  =   THIS_MODULE,
    .open   =   hello_open,     
    .write  =   hello_write,
};
 
static int __init hello_init(void){
    int ret;
    
    ret = register_chrdev(HELLO_MAJOR,DEVICE_NAME, &hello_flops);
    if (ret < 0) {
      printk(  " can't register major number.\n");
      return ret;
    }
    printk(" initialized. 3 \n");
    return 0;
}
 
static void __exit hello_exit(void){
    unregister_chrdev(HELLO_MAJOR, DEVICE_NAME);
    printk(KERN_INFO DEVICE_NAME " removed.2\n");
}
 
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("Dual BSD/GPL");