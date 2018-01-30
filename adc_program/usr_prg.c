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
	char volt_val[20];
	int fd=0;
	char dfilename[]="/dev/adc_device";


	// descriptor file open
	fd=open(dfilename, O_RDWR, S_IREAD|S_IWRITE);
	if(fd<0)
	{
		printf("Cannot open device file. Aborting !!\n");
		exit(-1);
	}

	// char driver write
	printf("Enter channel number:");
	scanf("%d",&channel);
	channel_num = '0' +(char)channel;
	write(fd, channel_num, sizeof(channel_num));

	// reading from char driver
	
	read(fd, &volt_val, sizeof(volt_val));
	printf("10 bit volt_val value:%s\n",volt_val);

	// file descriptor closing
	close(fd);
	return 0;
}
