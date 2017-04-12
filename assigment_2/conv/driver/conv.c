/*
 *  conv.c - Create an input/output character device
 */

#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for get_user and put_user */
#include <linux/init.h>
#include <linux/cdev.h> 
#include <linux/semaphore.h> 
#include <asm/uaccess.h> 
#include "chardev.h"
#define SUCCESS 0
#define DEVICE_NAME "char_dev"

#define height 512 //240
#define width  512 //320
#define depth  3   //1


unsigned char image [height*width*depth];
unsigned char *result;
int kernel;
unsigned char c_image [height][width][depth];


void kernel_method(int x, int d, int y){
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
//	unsigned char c_image [height][width][depth];
	int row, col,z;
	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			for (z = 0; z < depth; z++) {
//				printk(KERN_ALERT "Before to start\n");
				c_image[row][col][z] = image[(row * width + col) * depth + z];
			//	result[(row * width + col) * depth + z] = -1;
				printk(KERN_INFO "Info row %d col %d x %d image val %u\n",row,col,z,  image[(row * width + col) * depth + z]);

			}
		}
	}
	
	printk(KERN_INFO "Info image val %u\n", image[100]);
	printk(KERN_INFO "Info image val %u\n", image[1000]);
	printk(KERN_INFO "Info image val %u\n", image[10000]);
	printk(KERN_INFO "Info image val %u\n", image[2000]);
	printk(KERN_INFO "Info image val %u\n", image[600]);
	printk(KERN_INFO "Info image val %u\n", image[1300]);
	printk(KERN_INFO "Info image val %u\n", image[9120]);
	printk(KERN_INFO "Info image val %u\n", image[8400]);
	printk(KERN_INFO "Info image val %u\n", image[7430]);
	printk(KERN_INFO "Info image val %u\n", image[61240]);
	printk(KERN_INFO "Info image val %u\n", image[50230]);
	printk(KERN_INFO "Info image val %u\n", image[4330]);


}

/*
void kernel_left_sobel (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){

	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*1 + c_image[row-1][col][z]*0 + c_image[row-1][col+1][z]*-1 + 
						c_image[row]  [col-1][z]*2 + c_image[row]  [col][z]*0  + c_image[row]  [col+1][z]*-2 + 
						c_image[row+1][col-1][z]*1 + c_image[row+1][col][z]*0 + c_image[row+1][col+1][z]*-1);
}

void kernel_identity (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){

	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*0 + c_image[row-1][col][z]*0 + c_image[row-1][col+1][z]*0 + 
						c_image[row]  [col-1][z]*0 + c_image[row]  [col][z]*1  + c_image[row]  [col+1][z]*0 + 
						c_image[row+1][col-1][z]*0 + c_image[row+1][col][z]*0 + c_image[row+1][col+1][z]*0);
}

void kernel_outline (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){
	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*-1 + c_image[row-1][col][z]*-1 + c_image[row-1][col+1][z]*-1 + 
						c_image[row]  [col-1][z]*-1 + c_image[row]  [col][z]*8  + c_image[row]  [col+1][z]*-1 + 
						c_image[row+1][col-1][z]*-1 + c_image[row+1][col][z]*-1 + c_image[row+1][col+1][z]*-1);
}

void kernel_blur (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){
	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*0.0625 + c_image[row-1][col][z]*0.125 + c_image[row-1][col+1][z]*0.0625 + 
						c_image[row]  [col-1][z]*0.125 + c_image[row]  [col][z]*0.25  + c_image[row]  [col+1][z]*0.125 + 
						c_image[row+1][col-1][z]*0.0625 + c_image[row+1][col][z]*0.125 + c_image[row+1][col+1][z]*0.0625);
}

void kernel_sharpen (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){
	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*0 + c_image[row-1][col][z]*-1 + c_image[row-1][col+1][z]*0 + 
						c_image[row]  [col-1][z]*-1 + c_image[row]  [col][z]*5  + c_image[row]  [col+1][z]*-1 + 
						c_image[row+1][col-1][z]*0 + c_image[row+1][col][z]*-1 + c_image[row+1][col+1][z]*0);
}

void kernel_topsobel (uint8 *_proc_image, int h, int row, int w, int col, int depth, int z, uint8 c_image[h][w][depth]){
	_proc_image[(row * w + col) * depth + z] = (
						c_image[row-1][col-1][z]*1 + c_image[row-1][col][z]*2 + c_image[row-1][col+1][z]*1 + 
						c_image[row]  [col-1][z]*0 + c_image[row]  [col][z]*0  + c_image[row]  [col+1][z]*0 + 
						c_image[row+1][col-1][z]*-1 + c_image[row+1][col][z]*-2 + c_image[row+1][col+1][z]*-1);
}
*/
static int Device_Open = 0;

static int device_open(struct inode *inode, struct file *file){
	printk(KERN_INFO "device_open(%p)\n", file);
	// We don't want to talk to two processes at the same time 
	if (Device_Open)
		return -EBUSY;
	Device_Open++;
	//* Initialize the message 
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "device_release(%p,%p)\n", inode, file);
	// * We're now ready for our next caller 
	Device_Open--;
	module_put(THIS_MODULE);
	return SUCCESS;
}

static ssize_t device_write(struct file *file, unsigned char __user * buffer, size_t length, loff_t * offset)
{
	int i=0;
 	printk(KERN_INFO "Writting image into device \n");
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	copy_from_user(image , buffer , length);
//	char eso[5];
//	i = copy_from_user(eso , buffer , 5);
	printk(KERN_INFO "Value i is %d\n",i);

	return i;
}

static ssize_t device_read(struct file *file, char __user * buffer, size_t length, loff_t * offset)
{
	int i=0;
	printk(KERN_INFO "Reading image stored in the device \n");
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	i = copy_to_user(buffer, image, length);
	printk(KERN_INFO "value read from copy is %d\n", i);
	return i;

}

int device_ioctl(//struct inode *inode,	/* see include/linux/fs.h */
		 struct file *file,	/* ditto */
		 unsigned int ioctl_num,	/* number and param for ioctl */
		 unsigned long ioctl_param)
{
	int i=0;
	char message;

	/* 
	 * Switch according to the ioctl called 
	 */
		printk(KERN_INFO "IOCTL called with operation number:  %d\n", ioctl_num);

	switch (ioctl_num) {
	case IOCTL_SET_MODE:
		kernel = (int)ioctl_param;
		printk(KERN_INFO "SET MODE received\nKernel to apply is: %d\n",kernel);

		break;

	case IOCTL_START_CONV:
		printk(KERN_INFO "Starting kernel processing to the image...\n................]\n");
		kernel_method(0,0,0);
		break;

	case IOCTL_GET_NTH_BYTE:
		break;
	}

	return SUCCESS;
}

/* Module Declarations */

struct file_operations Fops = {
	.owner =          THIS_MODULE,
	.open =           device_open,
	.write =          device_write,
	.read =           device_read,
	.unlocked_ioctl = device_ioctl,
	.release =        device_release,	/* a.k.a. close */
};

// Initialize the module - Register the character device 
int init_module(){
	int ret_val;
	// Register the character device (atleast try) 
	ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

	 
	//  Negative values signify an error 
	if (ret_val < 0) {
		printk(KERN_ALERT "%s failed with %d\n", "Sorry, registering the character device ", ret_val);
		return ret_val;
	}

	printk(KERN_INFO "%s The major device number is %d.\n", "Registeration is a success", MAJOR_NUM);
	printk(KERN_INFO "mknod %s c %d 0\n", DEVICE_FILE_NAME, MAJOR_NUM);
	return 0;
}

// Cleanup - unregister the appropriate file from /proc 
void cleanup_module(){
	 // Unregister the device 
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	printk(KERN_ALERT "Succesfully un register the device\n");
}
