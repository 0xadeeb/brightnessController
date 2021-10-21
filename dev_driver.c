/*
Linux Device driver module that changes brightness of the screen on mosue scroll.
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
		printk("Error opening file\n");
		err = PTR_ERR(filp);
		return NULL;
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
	printk("Data - ( %s) written\n", data);
	return kernel_write(file, (void*)data, size, &offset);
}

int dev_open(struct inode *dev_inode, struct file *fp){

	times++;
	printk("Device opened %d times\n", times);
	
	return 0;

}

ssize_t dev_read(struct file *fp, char __user* buffer, size_t length, loff_t *offset){

	printk("inside funtion %s\n", __FUNCTION__);
	return 0;

}

ssize_t dev_write(struct file *fp, const char __user* buffer, size_t length, loff_t *offset){
	
	if(copy_from_user(buff, buffer, length) != 0){
		printk("Error opening write\n");
		return -EFAULT;
	}
	
	printk("Inside funtion %s, the data gotten is %s\n", __FUNCTION__, buff);
	int brightness = 0, i, n;
	char *brightnessController = "/sys/class/backlight/intel_backlight/brightness";
		
	filehandle1 = file_open(brightnessController, 0, 0);
	file_read(filehandle1, 0, brightness_buff, 6, brightnessController);
	printk("Current brightness: %s\n", brightness_buff);

	i = 0;
	while(i < 6 && (brightness_buff[i] >= 48 && brightness_buff[i] <58)){
		brightness*=10;
		brightness += (brightness_buff[i++] - 48);
	}

	if (buff[0] == '0')
	{
		printk("Up-scroll detected, Brightness increased\n");
		if (brightness < 7000)
			brightness += 500;
		printk(brightness_buff);
	}

	else if (buff[0] == '1')
	{
		printk("Down-scroll detected, Brightness decreased\n");
		if (brightness > 500)
			brightness -= 500;
		printk(brightness_buff);
	}

	else
	{
		printk("No data\n");
	}

	i = 0;
	memset(brightness_buff,	0, 10);
	memset(buff, 0, 10);
	while(brightness){
		brightness_buff[i++] = brightness%10 + 48;
		brightness/=10;
	}
	n = i-1;
	while(i >= 0){
		buff[n-i] = brightness_buff[i];
		i--;	
	}
	printk("Updated brightness: %s\n", buff);
	file_close(filehandle1);

	filehandle1 = file_open(brightnessController, O_WRONLY, 0);
	file_write(filehandle1, 0, buff, 6);
	file_close(filehandle1);

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