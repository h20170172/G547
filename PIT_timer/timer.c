#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>
#include<asm/io.h>
#include<linux/ktime.h>

#define timer_data 0x42   // channel 2
#define timer_control 0x43
#define timer_value 0xB6  //mode 2
#define num_value 597    // (1193/2 approx)
#define PIT_5 0x61 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prateek Garssa");
MODULE_DESCRIPTION("PIT Timer Module");

char devname[]="PIT_device";
char driver_name[]="PIT_Driver";
size_t ret_val=0;
struct class *dev_class;
struct cdev dev_struct;
int channel_num;
dev_t dev=0;
unsigned long int pit_delay=0;
int c;
int count=0;

void timer_delay(int value)
{
   u32 start,end;
   s64 actual_time;
    int dummy1,dummy2;
    dummy1=count + value ;
    start=ktime_get();
    while(count < dummy1)
     {
     if(((dummy2=inb(PIT_5)) & 0x20 ) == 0x20)
         if(((dummy2=inb(PIT_5)) & 0x20 ) == 0x00)
         {
	 count++;
	}
      }
	end=ktime_get();
  actual_time=ktime_to_ms(ktime_sub(end,start)); // for checking delay duration at kernal program
  printk(KERN_INFO"duration=%u",(unsigned int )actual_time);// not used in this programme, can be used for accuracy
 //     k=inb(0x42);
//	k=k | (inb(0x42)<< 8);// for checking square waveform
      
 //     printk(KERN_INFO"value is %d",(inb(PIT_5) & 0x20));
     
count=0;
}

int pit_timer_val(void)
{
int ret_value;
outb(0x20,timer_control);
ret_value = inb(timer_data);
ret_value=ret_value | (inb(timer_data) << 8 );
return ret_value;
}


ssize_t timer_read(struct file *filp, char __user *buf, size_t length, loff_t *offset)
{
   int p=inb(PIT_5);
   p=p|0x01;
   outb(p,PIT_5);
     timer_delay(pit_delay);
     c= pit_timer_val();
     printk(KERN_INFO"counter value %d",c);

	sprintf(buf,"%d",c);
	return 0;
}

ssize_t timer_write(struct file *filp, const char __user *buf, size_t length, loff_t *offset)
{

kstrtol(buf, 10, &pit_delay);
outb_p(timer_value,timer_control);
outb_p(num_value & 0xFF ,timer_data);
outb_p(num_value >> 8,timer_data);

return 0;
}

struct file_operations fops={
	.owner=THIS_MODULE,
	.read=timer_read,
	.write=timer_write,
};
 

int __init function_start(void)
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


void __exit function_exit(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&dev_struct);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "Driver unregistered !!\n");

printk(KERN_ALERT"leaving");
}

module_init(function_start);
module_exit(function_exit);


