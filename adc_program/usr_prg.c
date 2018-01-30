#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ioctl.h>

int main(void)
{
	int channel;
	char channel_num=' ';
	int num_bytes;
	char voltage[20];
	int fd=0;
	char full_device_file_name[]="/dev/adc_device";


	// opening file descriptor
	fd=open(full_device_file_name, O_RDWR, S_IREAD|S_IWRITE);
	if(fd<0)
	{
		printf("Cannot open device file. Aborting !!\n");
		exit(-1);
	}

	// writing to char driver
	printf("Enter channel number:");
	scanf("%d",&channel);
	if(channel>7||channel<0)
	{
		printf("Enter channel number between 0-7 !!\n");
		return -2;
	}
	channel_num = '0' +(char)channel;
	write(fd, channel_num, sizeof(channel_num));

	// reading from char driver
	
	read(fd, &voltage, sizeof(voltage));
	printf("10 bit voltage value:%s\n",voltage);

	// close the file descriptor
	close(fd);
	return 0;
}
