#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#define DEVICE_FILENAME	"/dev/mydev"

int main(int argc, char const *argv[])
{
	int dev;
	dev = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);
	ioctl(dev, 8, NULL);
	return 0;
}