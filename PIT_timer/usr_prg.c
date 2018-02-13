#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ioctl.h>

int main(void)
{
	int delay_ms;
	char channel_num=' ';
	char delay[20];
	char counter_val[20];
	int fd=0;
	char dfilename[]="/dev/PIT_device";


	// descriptor file open
	fd=open(dfilename, O_RDWR, S_IREAD|S_IWRITE);
	if(fd<0)
	{
		printf("Cannot open device file. Aborting !!\n");
		exit(-1);
	}

	// char driver write
	printf("Enter delay_ms number:");
	scanf("%d",&delay_ms);
	sprintf(delay,"%d",(delay_ms));
	write(fd, delay, sizeof(delay));

	// reading from char driver
	
	read(fd, &counter_val, sizeof(counter_val));
	printf("counter_val value:%s\n",counter_val);

	// file descriptor closing
	close(fd);
	return 0;
}
