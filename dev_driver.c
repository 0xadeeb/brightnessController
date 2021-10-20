/*
Linux Device driver module that changes brightness of the screen on click.
*/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/kdev_t.h>
#include <linux/unistd.h>
#include <linux/init.h>
// #include <linux/buffer_head.h>
// #include <asm/segment.h>
#include <asm/uaccess.h>

static int times = 0;
char brightness_buff[10], buff[10], *devName = "Mouse_Brightness_controller";
int k, btn_left, btn_right, btn_middle;
struct file *filehandle1, *filehandle2;

/*
The following functions help in reading/writing to files
within a Linux kernel module. File I/O should be avoided when possible.
Call VFS level functions instead of the syscall handler directly.
*/

struct file* file_open(const char* path, int flags, int rights)
{
	struct file* filp = NULL;
	int err = 0;
	filp = filp_open(path, flags, rights);
	if (IS_ERR(filp))
	{
		err = PTR_ERR(filp);
		return NULL;
		printk("Error opening file\n");
	}
	printk("File (%s) opened\n", path);
	return filp;
}

void file_close(struct file* file)
{
	filp_close(file, NULL);
}

int file_read(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size, char* location)
{
	int ret = kernel_read(file, (void*)data, size, &offset);
	printk("File (%s) read\n", location );
	return ret;
}

int file_write(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size)
{
	printk("Data - ( %s ) written\n", data);
	return kernel_write(file, (void*)data, size, &offset);
}

int dev_open(struct inode *dev_inode, struct file *fp){

	times++;
	printk("Device opened %d times\n", times);
	
	return 0;

}

ssize_t dev_read(struct file *fp, char __user* buffer, size_t length, loff_t *offset){

	printk("inside funtion %s\n", __FUNCTION__);

	int brightness = 0, i;
	const char *brightnessController = "/sys/class/backlight/intel_backlight/brightness";
	const char* mouseOutput = "/dev/input/mice";
	
	filehandle1 = file_open(brightnessController, 0, 0);
	file_read(filehandle1, 0, brightness_buff, 5, brightnessController);
	printk("Current brightness: %s\n", brightness_buff);

	filehandle2 = file_open(mouseOutput, 0, 0);
	file_read(filehandle2, 0, buff, 3, mouseOutput);
	printk("Mouse data = (%s)\n", buff);

	btn_left = buff[0] & 0x1;
	btn_right = buff[0] & 0x2;
	btn_middle = buff[0] & 0x04;

	i = 0;
	while(i < 5){
		if(brightness_buff[i] == 0) break;
		brightness*=10;
		brightness += brightness_buff[i++] - 48;
	}

	if (btn_left > 0)
	{
		printk("brightness_buff is left click:");
		if (brightness < 95) 
			brightness += 5;
		printk(brightness_buff);
	}

	else if (btn_right > 0)
	{
		printk("brightness_buff is right click:");
		if (brightness > 5)
			brightness -= 5;
		printk(brightness_buff);
	}

	else
	{
		printk("brightness_buff is middle click:");
		//do nothing
		printk(brightness_buff);
	}

	i = 0;	
	while(brightness){
		brightness_buff[i++] = brightness%10;
		brightness/=10;
	}

	file_close(filehandle1);

	filehandle1 = file_open(brightnessController, 1, 0);
	file_write(filehandle1, 0, brightness_buff, 5);
	file_close(filehandle1);
	printk("\n");
	return 0;

}

ssize_t dev_write(struct file *fp, const char __user* buffer, size_t length, loff_t *offset){
	
	printk("Inside funtion %s\n", __FUNCTION__);
	return length;

}


int dev_close(struct inode *dev_inode, struct file *fp){
	
	printk("inside funtion %s\n", __FUNCTION__);
	return 0;
}


static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.read = dev_read,
	.open = dev_open,
	.write = dev_write,
	.release = dev_close,
};


static int __init dev_init(void){

	if(register_chrdev(45, devName, &fops) < 0)
		printk("Failed\n");
	else
		printk("Device registered");
	return 0;
}

static void __exit dev_exit(void){
	printk("Device removed!");
	unregister_chrdev(45, devName);
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("Mouse Driver");
MODULE_AUTHOR("Custom");

