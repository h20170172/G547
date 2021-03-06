#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>
#include<linux/random.h>

// defining the IOCTL code

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prateek Garssa");
MODULE_DESCRIPTION("Getting random 10-bit value from ADC channel.");

char devname[]="adc_device";
char driver_name[]="ADC_Driver";
size_t ret_val=0;
struct class *dev_class;
struct cdev dev_struct;
unsigned int volt_val;
int channel_num;
dev_t dev=0;

static ssize_t adc_read(struct file *filp, char __user *buf, size_t length, loff_t *offset)
{
	unsigned int ret_val=0;
	printk(KERN_INFO "Driver:read() !!\n");
	printk(KERN_INFO "Channel_num:%d\n",channel_num);
	switch(channel_num)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			printk(KERN_INFO "Channel number:%d",channel_num);
			get_random_bytes(&volt_val, sizeof(unsigned int));
			break;
		default:
			printk(KERN_INFO "Enter valid channel number !!\n");
	}
	ret_val=volt_val%1024;
	sprintf(buf,"%u",ret_val);
	return 0;
}

static ssize_t adc_write(struct file *filp, const char __user *buf, size_t length, loff_t *offset)
{
	printk(KERN_INFO "Driver:write() !!\n");
	channel_num=(int)buf-'0';
	return 0;
}

static int adc_open(struct inode *inodep,struct file *filp)
{
	printk(KERN_INFO "Driver:open() !!\n");
	return 0;
}

static int adc_close(struct inode *inodep,struct file *filp)
{
	printk(KERN_INFO "Driver:close() !!\n");
	return 0;
}

struct file_operations fops={
	.owner=THIS_MODULE,
	.read=adc_read,
	.write=adc_write,
	.open=adc_open,
	.release=adc_close
};

int function_start(void)
{
	// allocating major/minor number
	if(alloc_chrdev_region(&dev,0,1,driver_name)<0)
	{
		printk(KERN_INFO "Cannot add device to system. Aborting !!\n");
		return -1;
	}
	
	// creating cdev structure
	cdev_init(&dev_struct,&fops);
	dev_struct.owner=THIS_MODULE;
	dev_struct.ops=&fops;

	// adding character device to the system
	if(cdev_add(&dev_struct,dev,1)<0)
	{
		printk(KERN_INFO "Cannot add device to the system !!\n");
		goto r_class;
	}

	// creating struct class
	if((dev_class=class_create(THIS_MODULE,"ext_class")) == NULL)
	{
		printk(KERN_INFO "Cannot create struct class !!\n");
		goto r_class;
	}

	// creating device
	if((device_create(dev_class,NULL,dev,NULL,devname))==NULL)
	{
		printk(KERN_INFO "Cannot create device file !!\n");
		goto r_device;
	}
	printk(KERN_INFO "Device file created successfully !!\n");

	return 0;

	r_device:
		class_destroy(dev_class);
	r_class:
		unregister_chrdev_region(dev,1);

	return -1;
}

void function_end(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&dev_struct);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "Driver unregistered !!\n");
}

module_init(function_start);
module_exit(function_end);
