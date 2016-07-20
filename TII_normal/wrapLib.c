/* 
 * Created By KUOS
 *
 * < Wrapper for openGL ES 2.0 functions in libGLESv2.so >
 *
 * Shape of all wrapper functions is same to the original openGL ES 2.0 functions
 * The body of each function is based on follows:
 *         1) Copy parameters to local buffer 
 *         2) Call write function to write the shared memory between worlds
 *         3) Read return values from the shared memory between worlds if it is required
 *         4) Return to the user application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include "wrapLib_struct.h"
#include <time.h>

int dev = -1;
int Ret = -1;

char wbuf[MAX_BUF_SIZE] ={0, };
char rbuf[MAX_BUF_SIZE] ={0, };

struct space_list *unfreed = NULL;
struct space_list *unfreed_tail = NULL;

#ifdef MEASURE_TIME
#define RESULT_FILE "/root/time_result"
#endif


#undef TZ_DEBUG_LEVEL_2U
#undef TZ_DEBUG_LEVEL_3U
#undef TZ_DEBUG_LEVEL_10
/*
 * -------------------------------------
 * Utility functions to support stub functions
 */


void add_to_unfreed_list(struct space_list *header, struct space_list *tail, void *addr)
{
	struct space_list *tmp = NULL;
	
	if(header == NULL) {
		header = (struct space_list *)malloc(sizeof(struct space_list));
		tail = header;
		tail->next = NULL;
	}
	else {
		tmp = (struct space_list *)malloc(sizeof(struct space_list));
		tmp->buf = addr;
		tmp->next = NULL;

		tail->next = tmp;
		tail = tmp;
	}
}

void free_unfreed_list(struct space_list *header)
{
	struct space_list *tmp = NULL;
	
	while(header != NULL) {
		tmp = header;
		header = header->next;

		free(tmp->buf);
		free(tmp);
	}
}


/*
 * Function to read from large data buffer
 * This function is created to avoid overflow if the size of buf is smaller than count
 */
int read_without_overflow(int fp, void *buf, int count)
{
	return read(fp, buf, count);
}

/*
 * Function to read from default data buffer that is mapped to the shared memory
 * This function Periodically read data
 */
int read_from_shmem(int filp, void *buf, int count)
{
	int ret = -1;

	/*
	struct timespec reqtime;
	reqtime.tv_sec = 0;
	reqtime.tv_nsec = 5;
	*/

	do {
		ret = read(filp, buf, count);

		if(ret > 0)
			break;
		else
			usleep(1);
			//nanosleep(&reqtime, NULL);
	} while(ret <= 0);

	return ret;
}

/*
 * -------------------------------------
 * Wrapper functions
 */

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

int tzbench_stop_tmr(int *upper_cnt, int *lower_cnt)
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

ssize_t tzwrite(int in_fd, void *in_buf, size_t in_count)
{
	str_tzwrite *writebuf;
//	str_tzwrite *returnbuf;
//	size_t rest_count;
//	unsigned int in_buf_pos;

#if defined(TZ_DEBUG_LEVEL_2U) ||defined(TZ_DEBUG_LEVEL_3U) ||  defined(TZ_DEBUG_LEVEL_10)
	int tzbench_upper_cnt = 0;
	int tzbench_lower_cnt = 0;
#endif 

#ifdef TZ_DEBUG_LEVEL_2U
	tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL_2U

//	in_buf_pos = 0;

	writebuf = (str_tzwrite*)wbuf;
//	returnbuf = (str_tzwrite*)rbuf;

//	init_header(&(writebuf->hd));
//	init_header(&(returnbuf->hd));
	init_msg(writebuf);

//	writebuf->hd.funcID = ID_tzwrite;
	writebuf->request_op = ID_tzwrite;
	writebuf->retFlag = RT_REQUEST;
	writebuf->file_desc = in_fd;
	writebuf->buffer_PA = in_buf;
	writebuf->buffer_len = in_count;

//	rest_count = in_count;



	Ret = write(dev, writebuf, MAX_BUF_SIZE);

/*
* 	SWKIM
*  이제 여기에서 pause()를 하여 signal을 기다리자.
*  signal이 들어 오면, 그때 리턴을 하자.
*/

		call_log(Ret);

#ifdef TZ_DEBUG_LEVEL_2U
	tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);
	printf("Dev_share write function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_2U
#ifdef TZ_DEBUG_LEVEL_3U
	tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL_3U

/*
* 	SWKIM
*  이제 read_from_shmem()은 이제 안쓴다.
*  지우자.
*/



#ifdef TZ_DEBUG_LEVEL_3U
	tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);	
	printf("Dev_share write function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_3U

#ifdef TZ_DEBUG_LEVEL_10
	tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);	
	printf("Dev_share write function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_10
	return writebuf->ret_val;


}

ssize_t tzread(int in_fd, void *in_buf, size_t in_count)
{
	str_tzread *writebuf;
//	str_tzread *returnbuf;
	size_t rest_count;
	unsigned int in_buf_pos;

#if defined(TZ_DEBUG_LEVEL_2U) ||defined(TZ_DEBUG_LEVEL_3U) ||  defined(TZ_DEBUG_LEVEL_10)
	int tzbench_upper_cnt = 0;
	int tzbench_lower_cnt = 0;
#endif 

#ifdef TZ_DEBUG_LEVEL_2U
	tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL_2U
	in_buf_pos = 0;

	writebuf = (str_tzread*)wbuf;
//	returnbuf = (str_tzread*)rbuf;

//	init_header(&(writebuf->hd));
//	init_header(&(returnbuf->hd));
	init_msg(writebuf);

//	writebuf->hd.funcID = ID_tzread;
	writebuf->request_op = ID_tzread;
	writebuf->retFlag = RT_REQUEST;
	writebuf->file_desc = in_fd;
	writebuf->buffer_PA = in_buf;
	writebuf->buffer_len = in_count;

	rest_count = in_count;

/*	
	while(rest_count && in_buf_pos < in_count){

		memset(writebuf->buf, 0, MAX_RW_BUF_SIZE);
		memset(returnbuf->buf, 0, MAX_RW_BUF_SIZE);
		if(rest_count < MAX_RW_BUF_SIZE){
			//요청한 read buf 크기가 MAX_BUF_SIZE보다 작으므로 마지막(1번의) read 요청을 보낸다.
			writebuf->read_count = rest_count;
						
		}else {
			//요청한 read할 데이터 크기가 MAX_RW_BUF_SIZE보다 크므로, 여러번 read 요청을 보낸다.
			writebuf->read_count = MAX_RW_BUF_SIZE;
		}
*/


/*
* 	SWKIM
*  이제 여기에서 pause()를 하여 signal을 기다리자.
*  signal이 들어 오면, 그때 리턴을 하자.
*/


		Ret = write(dev, writebuf, MAX_BUF_SIZE);
//		call_log(Ret);
		
#ifdef TZ_DEBUG_LEVEL_2U
		tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);
		printf("Dev_share read function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_2U
#ifdef TZ_DEBUG_LEVEL_3U
		tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL3U

/*
* 	SWKIM
*  이제 read_from_shmem()은 이제 안쓴다.
*  지우자.
*/

		//read 요청 처리 결과 수신
//		returnbuf->hd.funcID = ID_tzread;

//		Ret = read_from_shmem(dev, returnbuf, MAX_BUF_SIZE);
/*
		if(returnbuf->returnval <= 0) {
			printf("Read error returnbuf->returnval is %d\n", returnbuf->returnval);
			break;
		}
*/		
		//printf("read from secure world success\n");
//		memcpy(in_buf + in_buf_pos, returnbuf->buf, returnbuf->returnval);

		//in_buf_pos을 수신한 size만큼 증가
//		in_buf_pos = in_buf_pos + returnbuf->returnval;

		//수신 된 size 만큼 rest_cout 감소
//		rest_count = rest_count  - returnbuf->returnval;
//	}


#ifdef TZ_DEBUG_LEVEL_3U
		tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);
		printf("Dev_share read function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_3U

#ifdef TZ_DEBUG_LEVEL_10
		tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);	
		printf("Dev_share read function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_10

	return writebuf->ret_val;


}

int tzopen(const char *pathname, int flags)
{

	str_tzopen *writebuf;
//	str_tzopen *returnbuf;

#if defined(TZ_DEBUG_LEVEL_2U) ||defined(TZ_DEBUG_LEVEL_3U) ||  defined(TZ_DEBUG_LEVEL_10)
	int tzbench_upper_cnt = 0;
	int tzbench_lower_cnt = 0;
#endif 

#ifdef TZ_DEBUG_LEVEL_2U
	tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL_2U


	writebuf = (str_tzopen*)wbuf;
//	returnbuf = (str_tzopen*)rbuf;


//	init_header(&(writebuf->hd));
//	init_header(&(returnbuf->hd));
	init_msg(writebuf);

//	writebuf->hd.funcID = ID_tzopen;
//	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->retFlag = RT_REQUEST;
	writebuf->request_op = ID_tzopen;
	memcpy(writebuf->file_path, pathname, strlen(pathname));
	writebuf->file_open_flag =flags;

//	writebuf->flags = flags;


/*
* 	SWKIM
*  이제 여기에서 pause()를 하여 signal을 기다리자.
*  signal이 들어 오면, 그때 리턴을 하자.
*/	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);

#ifdef TZ_DEBUG_LEVEL_2U
	tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);
	printf("Dev_share open function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_2U
#ifdef TZ_DEBUG_LEVEL_3U
	tzbench_start_tmr();
#endif
	call_log(Ret);

/*
* 	SWKIM
*  이제 read_from_shmem()은 이제 안쓴다.
*  지우자.
*/

//	returnbuf->hd.funcID = ID_tzopen;
//	Ret = read_from_shmem(dev, returnbuf, MAX_BUF_SIZE);

#ifdef TZ_DEBUG_LEVEL_3U
	tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);
	printf("Dev_share open function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_3U

#ifdef TZ_DEBUG_LEVEL_10
	tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);	
	printf("Dev_share open function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_10

	return writebuf->ret_val;
}

int tzclose(int in_fd)
{

	str_tzclose *writebuf;
//	str_tzclose *returnbuf;

#if defined(TZ_DEBUG_LEVEL_2U) ||defined(TZ_DEBUG_LEVEL_3U) ||  defined(TZ_DEBUG_LEVEL_10)
	int tzbench_upper_cnt = 0;
	int tzbench_lower_cnt = 0;
#endif 

#ifdef TZ_DEBUG_LEVEL_2U
	tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL_2U

	writebuf = (str_tzclose*)wbuf;
//	returnbuf = (str_tzclose*)rbuf;

//	init_header(&(writebuf->hd));
//	init_header(&(returnbuf->hd));
	init_msg(writebuf);
	writebuf->request_op = ID_tzclose;
//	writebuf->hd.funcID = ID_tzclose;
//	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->retFlag = RT_REQUEST;

	//printf("tzclose ID is %d\n", writebuf->hd.funcID);
	writebuf->file_desc = in_fd;
//	writebuf->fd = in_fd;
/*
* 	SWKIM
*  이제 여기에서 pause()를 하여 signal을 기다리자.
*  signal이 들어 오면, 그때 리턴을 하자.
*/
	Ret = write(dev, writebuf, MAX_BUF_SIZE);

#ifdef TZ_DEBUG_LEVEL_2U
	tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);
	printf("Dev_share close function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_2U
#ifdef TZ_DEBUG_LEVEL_3U
	tzbench_start_tmr();
#endif //TZ_DEBUG_LEVEL_3U

/*
* 	SWKIM
*  이제 read_from_shmem()은 이제 안쓴다.
*  지우자.
*/
//	Ret = read_from_shmem(dev, returnbuf, MAX_BUF_SIZE);

	//printf("write return val")
//	call_log(Ret);

//	returnbuf->hd.funcID = ID_tzclose;


#ifdef TZ_DEBUG_LEVEL_3U
	tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);
	printf("Dev_share close function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_3U

#ifdef TZ_DEBUG_LEVEL_10
	tzbench_stop_tmr(&tzbench_upper_cnt, &tzbench_lower_cnt);	
	printf("Dev_share close function %d %d\n",tzbench_upper_cnt, tzbench_lower_cnt);
#endif //TZ_DEBUG_LEVEL_10

	return writebuf->ret_val;
}
