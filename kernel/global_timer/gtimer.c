/*
 * Created by KUOS
 *
 * This file provides a new communication mechanism between secure world and normal world.
 * ipi_write() writes data into a shared memory between worlds, and notifies to another world.
 * ipi_read() reads data from the shared memory if receive flag value is set.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>          
//#include <linux/errno.h>       
//#include <linux/types.h>       
#include <linux/fcntl.h>       
#include <asm/uaccess.h>
#include <asm/io.h>
//#include <linux/stat.h>
//#include <linux/sched.h>
//#include <linux/string.h>
//#include <mach/io.h>
//#include <linux/time.h>
#include <linux/unistd.h>
//#include <linux/delay.h>

#define CALL_DEV_NAME "mydev"
#define CALL_DEV_MAJOR 240 

#define GLOBAL_TIMER_START 3
#define GLOBAL_TIMER_STOP 4
#define GET_LOWER_TIME_CNT 5
#define GET_UPPER_TIME_CNT 6

int upper = 0;
int lower = 0;

/*
 * Function to set various flag values
 *      GLOBAL_TIMER_~	: Set and reset the global timer for time measurement
 */
long gtimer_ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
	int *reg_addr = NULL;
	int *addr = NULL;

	switch (cmd) {
	case GLOBAL_TIMER_START:
		// reset timer values
		reg_addr = (int*)0xf2a00200;	// lower 32bits
		*reg_addr = 0x0;
		reg_addr = (int*)0xf2a00204;	// upper 32bits
		*reg_addr = 0x0;
		// start timer
		reg_addr = (int*)0xf2a00208;
		*reg_addr = 0x1;
		break;
	case GLOBAL_TIMER_STOP:
		reg_addr = (int*)0xf2a00208;
		*reg_addr = 0x0;
		// reset timer values
		reg_addr = (int*)0xf2a00200;	// lower 32bits
		lower = *reg_addr;
		*reg_addr = 0x0;
		reg_addr = (int*)0xf2a00204;	// upper 32bits
		upper = *reg_addr;
		*reg_addr = 0x0;
		break;
	case GET_LOWER_TIME_CNT:
		addr = (int*)arg;
		copy_to_user(addr, &lower, sizeof(int));
		break;
	case GET_UPPER_TIME_CNT:
		addr = (int*)arg;
		copy_to_user(addr, &upper, sizeof(int));
		break;
	}
	
	return 0;
}

/*
 * Function to open the device
 */
int gtimer_open(struct inode *inode, struct file *filp){
	return 0;
}

/*
 * Function to close the device
 */
int gtimer_release(struct inode *inode, struct file *filp){
	return 0;
}

//connect each device call to specific function
struct file_operations gtimer_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl		= gtimer_ioctl,
	.open			= gtimer_open,
	.release		= gtimer_release,
};

/*
 * Function to initialize the device
 */
int gtimer_init(void){
	int result = 0;
	printk(KERN_DEBUG "Global Timer : init called!\n");

	//Register the character device
	result = register_chrdev(CALL_DEV_MAJOR, CALL_DEV_NAME, &gtimer_fops);
	if (result < 0){
		printk("fail to init module: %d\n", result);
		return result;
	}

	return 0;
}

/*
 * Function to release the device
 */
void gtimer_exit(void){
	printk(KERN_EMERG "Global Timer : exit called!\n");
	unregister_chrdev(CALL_DEV_MAJOR, CALL_DEV_NAME);
}

module_init(gtimer_init);
module_exit(gtimer_exit);

