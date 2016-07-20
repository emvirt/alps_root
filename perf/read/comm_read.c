#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEVICE_FILENAME	"/dev/mydev"
#define MAX_DEFAULT_INT_COUNT 128
#define MAX_BUF_SIZE 512
#define MAX_LARGE_BUF_SIZE 0x100000U	// 1024 * 1024

// ioctl commands
#define IMMEDIATE_SEND_START 0
#define IMMEDIATE_SEND_STOP 1
#define GLOBAL_TIMER_START 3
#define GLOBAL_TIMER_STOP 4

int read_from_shmem(int filp, void *buf, int count, int utime)
{
	int ret = -1;

	do {
		ret = read(filp, buf, count);

		if(ret > 0)
			break;
		else
			usleep(utime);
	} while(ret <= 0);

	return ret;
}

int read_without_overflow(int filp, void *buf, int count)
{
	return read(filp, buf, count);
}

int use_default_buffer(int bytes, int utime)
{
	int data[MAX_DEFAULT_INT_COUNT];
	int dev = -1;
	int ret = 0;

	dev = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);
	if(dev < 0)
		return -1;

	ret = read_from_shmem(dev, data, bytes, utime);

	// end timer
	ret = ioctl(dev, GLOBAL_TIMER_STOP);

	close(dev);

	return 0;
}

int use_large_buffer(int bytes, int utime)
{
	void *data;
	void *cur;
	int header[4];
	int dev = -1;
	int ret = 0;
	int header_size = sizeof(header);
	int data_size = bytes - header_size;
	int data_count = data_size / MAX_LARGE_BUF_SIZE;
	int remained_data_size = data_size % MAX_LARGE_BUF_SIZE;
	int i=0;

	data = malloc(bytes);
	memset(data, 0, bytes);

	dev = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);
	if(dev < 0)
		return -1;

	header[0] = 1;	// funcID = 1
	header[1] = 2;	// retFlag = response

	ret = read_from_shmem(dev, header, header_size, utime);

	cur = data;
	for(i=0; i<data_count; i++) {
		ret = write(dev, header, header_size);

		ret = read_without_overflow(dev, cur, (MAX_LARGE_BUF_SIZE + MAX_BUF_SIZE));
		cur = cur + MAX_LARGE_BUF_SIZE;
	}
	if(remained_data_size > 0) {
		ret = write(dev, header, header_size);

		ret = read_without_overflow(dev, cur, (remained_data_size + MAX_BUF_SIZE));
	}

	// end timer
	ret = ioctl(dev, GLOBAL_TIMER_STOP);

	close(dev);
	free(data);

	return 0;
}

int main (int argc, char *argv[])
{
	int bytes = 0;
	int end_of_num = 0;
	int end_of_str = 0;
	char last_ch;
	int unit = 1;

	int sleep_utime = 0;

	if(argc < 3) {
		printf("arguments are required\n");
		return 0;
	}

	end_of_str = strlen(argv[1]) - 1;
	end_of_num = end_of_str - 1;
	if(argv[1][end_of_num] < '0' || argv[1][end_of_num] > '9') {
		printf("Wrong argument: first argument is not number\n");
		return 0;
	}

	last_ch = argv[1][end_of_str];
	if(last_ch < '0' || last_ch > '9') {
		if(last_ch == 'm' || last_ch == 'M') {
			argv[1][end_of_str] = '\0';
			unit = 1024 * 1024;
		}
		else if(last_ch == 'k' || last_ch == 'K') {
			argv[1][end_of_str] = '\0';
			unit = 1024;
		}
		else {
			printf("Wrong argument: the unit of first argument is wrong\n");
			return 0;
		}
	}

	bytes = atoi(argv[1]) * unit;

	sleep_utime = atoi(argv[2]);

	if(bytes < 0) {
		printf("Wrong size: \"bytes\" < 0\n");
		return 0;
	}
	else if(bytes <= MAX_BUF_SIZE)
		use_default_buffer(bytes, sleep_utime);
	else
		use_large_buffer(bytes, sleep_utime);

	return 0;
}
