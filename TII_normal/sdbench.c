#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#ifdef SHARED_DEVICE
#define open tzopen
#define close tzclose
#define read tzread
#define write tzwrite
int tzbench_init(void);
int tzbench_free(void);
int tzopen(const char *pathname, int flags);
int tzclose(int in_fd);
ssize_t tzread(int in_fd, void *in_buf, size_t in_count);
ssize_t tzwrite(int in_fd, void *in_buf, size_t in_count);
int tzbench_start_tmr(void);
int tzbench_stop_tmr(unsigned long *upper_cnt, unsigned long *lowwer_cnt);
#else
int dev = -1;
#define DEVICE_FILENAME	"/dev/mydev"

/* 
 * Define ioctl commands
 */
#define IMMEDIATE_SEND_START 0
#define IMMEDIATE_SEND_STOP 1
#define GLOBAL_TIMER_START 3
#define GLOBAL_TIMER_STOP 4
#define GET_LOWER_TIME_CNT 5
#define GET_UPPER_TIME_CNT 6
#define SET_SIGNAL_INFO 7


int tzbench_init(void)
{

	dev = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);
	//printf("device open. dev = %d\n", dev);
	return 0;
}

int tzbench_start_tmr(void)
{
	ioctl(dev, GLOBAL_TIMER_START, NULL);

	return 0;
}

int tzbench_stop_tmr(unsigned long *upper_cnt, unsigned long *lower_cnt)
{
	ioctl(dev, GLOBAL_TIMER_STOP, NULL);
	ioctl(dev, GET_UPPER_TIME_CNT, upper_cnt);
	ioctl(dev, GET_LOWER_TIME_CNT, lower_cnt);	
	return 0;
}

int tzbench_free(void)
{
	close(dev);
	return 0;

}
#endif

#define READ_BENCH 1
#define WRITE_BENCH 2
#define RW_BENCH 3

//#define TZ_DEBUG_LEVEL_1

int main(int argc, char** argv) 
{
	int fd;
	//int out_fd;
	char filename[100]={0,};
	//char filename2[100]={0,};
	int bench_op = 0;
	int bench_size = 0;
	char* buf = 0, *out_buf = 0;
	//struct timeval start_time, end_time;
//	unsigned long tmr_upper_time, tmr_lower_time;
//	double exec_t_sec, exec_t_usec;

	if(argc < 4) {
		printf("\nUsage ./bench <filename> <operation> <size>\n Example: ./bench mybenchfile RW 1000\n\n");
		return 0;
	}
	//Copy file name
	strncpy(filename, argv[1], strlen(argv[1]) );
	//printf("filename %s %lu %lu\n",filename, strlen(filename), strlen(argv[1]));

	

	if(strncmp(argv[2],"R",1) == 0 ) {
		bench_op = READ_BENCH;
	}else if (strncmp(argv[2],"W",1) == 0) {
		bench_op = WRITE_BENCH;
	}else if (strncmp(argv[2],"B",1) == 0) {
		bench_op = RW_BENCH;
	}else {
		printf("No operation defined\n");
		return 0;
	}

	bench_size = atoi(argv[3]);
	
	if(bench_size != 0) {
		buf = (char*)malloc(bench_size);
		out_buf = (char*)malloc(bench_size);
		memset(out_buf, 'A', bench_size);
	}



	//gettimeofday(&start_time, NULL);
	
	//#ifdef SHARED_DEVICE
	tzbench_init();

#ifdef TZ_DEBUG_LEVEL_1
	tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL_1
	//#endif

	
	fd = open(filename,O_RDWR);


#ifdef TZ_DEBUG_LEVEL_1
	tzbench_stop_tmr(&tmr_upper_time, &tmr_lower_time);
#ifdef SHARED_DEVICE
	printf("Dev_share open function %s %d B  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
#else
	printf("Direct        open function %s %d B  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
#endif
#endif //TZ_DEBUG_LEVEL_1
	
	
	if (fd < 0) {
#ifdef SHARED_DEVICE
		printf("input_file tzopen fail\n");
#else
		printf("input_file close fail\n");
#endif
	}

		
	switch(bench_op)
	{
		case READ_BENCH:

#ifdef TZ_DEBUG_LEVEL_1
		tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL_1
	
		read(fd, buf, bench_size);

#ifdef TZ_DEBUG_LEVEL_1
		tzbench_stop_tmr(&tmr_upper_time, &tmr_lower_time);
#ifdef SHARED_DEVICE
		printf("Dev_share read function %s %d B  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
#else
		printf("Direct        read function %s %d B  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
#endif
#endif //TZ_DEBUG_LEVEL_1
		
		break;

		case WRITE_BENCH:

#ifdef TZ_DEBUG_LEVEL_1
		tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL_1

		write(fd, out_buf, bench_size);
		
#ifdef TZ_DEBUG_LEVEL_1
		tzbench_stop_tmr(&tmr_upper_time, &tmr_lower_time);
#ifdef SHARED_DEVICE
		printf("Dev_share write function %s %d B  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
#else
		printf("Direct        write function %s %d B  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
#endif
#endif //TZ_DEBUG_LEVEL_1

		break;

		case RW_BENCH:
#ifdef TZ_DEBUG_LEVEL_1
		tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL_1
		read(fd, buf, bench_size);
		write(fd, buf, bench_size);
#ifdef TZ_DEBUG_LEVEL_1
		tzbench_stop_tmr(&tmr_upper_time, &tmr_lower_time);
		
#ifdef SHARED_DEVICE
		printf("Dev_share RW function %s %d B  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
#else
		printf("Direct        RW function %s %d B  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
#endif
#endif //TZ_DEBUG_LEVEL_1


		break;

		default:
		printf("No bench defined\n");
		return 0;
	}

#ifdef TZ_DEBUG_LEVEL_1
	tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL_1
	close(fd);

#ifdef TZ_DEBUG_LEVEL_1
	tzbench_stop_tmr(&tmr_upper_time, &tmr_lower_time);
		
#ifdef SHARED_DEVICE
	printf("Dev_share close function %s %d KB  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
#else
	printf("Direct        close function %s %d KB  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
#endif
#endif //TZ_DEBUG_LEVEL_1
	//close(out_fd);
	
	//#ifdef SHARED_DEVICE
	//tzbench_stop_tmr(&tmr_upper_time, &tmr_lower_time);
	tzbench_free();
	//#endif
	
	/*
	#ifdef SHARED_DEVICE
	printf("Dev_share %s %d KB  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
	
	#else
	printf("Direct %s %d KB  %lu  %lu\n",argv[2], bench_size, tmr_upper_time, tmr_lower_time );
	#endif
	*/
	free(buf);
	free(out_buf);
	return 0;
}
