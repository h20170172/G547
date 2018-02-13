This Device Driver code will execute delay in milliseconds through 8254 Programmable Interval Timer

 
Follow these steps to build the program:

STEP : 1
$ make all
$ sudo insmod timer.ko

This will load the device driver into kernel.

STEP : 2
$ gcc usr_prg.c -o usr_p
$ change permission for access file(chmod 777)
$ sudo ./usr_p

The program will ask to enter delay in ms:
a. Insert delay (in milliseconds)
b. Read current timer counter value.
 .
