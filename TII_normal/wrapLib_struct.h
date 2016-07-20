/* Created By KUOS */

#include "wrapLib_define.h"

/*
 * Structures for OpenGL ES 2.0 functions.
 * These structures include 
 *         a common header for function identification, 
 *         a return value, 
 *         and parameters.
 * Layout : 
 *         str_$(OpenGL ES function name)
 */

#define MAX_RW_BUF_SIZE MAX_BUF_SIZE-sizeof(typ_common_hd)-20

typedef struct _tz_msg{
	int request_op;
	void* buffer_PA;
	size_t buffer_len;
	int file_open_flag;
	int retFlag;
	int file_desc;
	int ret_val;
	char file_path[36];
}tz_msg, str_tzwrite, str_tzread, str_tzopen, str_tzclose;

#if 0
typedef struct struct_tzwrite{
	typ_common_hd hd;

	int fd;
	size_t write_count;
	char buf[MAX_RW_BUF_SIZE];
	int returnval;
}str_tzwrite;

typedef struct struct_tzread{
	typ_common_hd hd;

	int fd;
	size_t read_count;
	char buf[MAX_RW_BUF_SIZE];
	int returnval;
}str_tzread;


 typedef struct struct_tzclose{
	typ_common_hd hd;

	int fd;
	int returnval;
}str_tzclose;


 typedef struct struct_tzopen{
	typ_common_hd hd;

	char pathname[128];
	int flags;
	int returnval;
}str_tzopen;
 
#endif