#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "eglwrap_struct.h"

extern int read_from_shmem(int filp, void *buf, int count);
extern int read_without_overflow(int fp,void * buf,int count);
extern void free_unfreed_list(struct space_list *header);

extern int dev;

extern char wbuf[MAX_BUF_SIZE];
extern char rbuf[MAX_BUF_SIZE];

extern struct space_list *unfreed;

// return array for the string type
char ret_string[128] = {0, };

//init function
EGLDisplay eglGetDisplay(EGLNativeDisplayType eglNativeDisplayType)
{
	int Ret;
	str_eglGetDisplay *writebuf;
	str_eglGetDisplay *readbuf;
	EGLDisplay eglDisplay;

	writebuf = (str_eglGetDisplay*)wbuf;
	readbuf = (str_eglGetDisplay*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));
	
	writebuf->hd.funcID = ID_eglGetDisplay;
	writebuf->hd.retFlag = 1;
	writebuf->param_eglNativeDisplayType = eglNativeDisplayType;

/*
	if(dev<0)
	{
		dev = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);
		printf("device open-> dev = %d\n", dev);
	}
*/

	Ret = write(dev, writebuf, MAX_BUF_SIZE);

	readbuf->hd.funcID = ID_eglGetDisplay;
	
	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	eglDisplay = readbuf->retVal;

	call_log(Ret);

//	close(dev);
	return eglDisplay;
}

EGLBoolean eglInitialize(EGLDisplay display, EGLint* major, EGLint* minor)
{
	int Ret;
	EGLBoolean result;
	str_eglInitialize *writebuf;
	str_eglInitialize *readbuf;

	writebuf = (str_eglInitialize*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglInitialize;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->display = display;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
	
	readbuf = (str_eglInitialize*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglInitialize);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	if(major != NULL)
		*major = readbuf->major;
	if(minor != NULL)
		*minor = readbuf->minor;
	result = readbuf->retVal;

	return result;
}

EGLBoolean eglBindAPI(EGLenum api)
{
	int Ret;
	EGLBoolean result;
	str_eglBindAPI *writebuf;
	str_eglBindAPI *readbuf;

	writebuf = (str_eglBindAPI*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglBindAPI;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->api = api;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglBindAPI*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglBindAPI);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

EGLBoolean eglChooseConfig(EGLDisplay display, EGLint const* attrib_list, EGLConfig* configs, EGLint config_size, EGLint* num_config)
{
	int Ret, i=0;
	EGLBoolean result;
	void *buf = NULL;
	str_eglChooseConfig *writebuf;
	str_eglChooseConfig *readbuf;

//	EGLint attrib_count = PARAM_SIZE(eglChooseConfig) / sizeof(EGLint);
	EGLint struct_length = sizeof(str_eglChooseConfig);
	EGLint config_length = 0;
	EGLint total_length = 0;

	writebuf = (str_eglChooseConfig*)wbuf;
//	init_header(&(writebuf->hd));
	memset(writebuf, 0, struct_length);

	writebuf->hd.funcID = ID_eglChooseConfig;
	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->display = display;
	writebuf->config_size = config_size;
	if(configs == NULL)
		writebuf->configs_buf_offset = 0;
	else
		writebuf->configs_buf_offset = struct_length;
//	memset((writebuf->attrib_list), 0, PARAM_SIZE(eglChooseConfig));
	writebuf->attrib_list[0] = EGL_NONE;
	if(attrib_list != NULL) {
		while(attrib_list[i]!=EGL_NONE)
			i++;
		writebuf->attrib_count = i + 1;	// include EGL_NONE
		memcpy(writebuf->attrib_list, attrib_list, (sizeof(EGLint)*writebuf->attrib_count));
	}

//	if(attrib_list != NULL)
//		memcpy(writebuf->attrib_list, attrib_list, PARAM_SIZE(eglChooseConfig));
/*	if(attrib_list != NULL) {
		for(i=0; attrib_list[i]!=EGL_NONE; i++) {
			writebuf->attrib_list[i] = attrib_list[i];
			if(i >= attrib_count - 1)
				break;
		}
		writebuf->attrib_list[i] = EGL_NONE;
	}
*/
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglChooseConfig*)rbuf;
	readbuf->hd.funcID = ID_eglChooseConfig;
	
	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	if(readbuf->hd.ldata_length > 0) {
		total_length = readbuf->hd.ldata_length + COMM_HEAD_SIZE;
		
		buf = (void*)malloc(total_length);
		memset(buf, 0, total_length);
		memcpy(buf, readbuf, COMM_HEAD_SIZE);

		Ret = read_without_overflow(dev,(buf + COMM_HEAD_SIZE),(readbuf->hd.ldata_length + MAX_BUF_SIZE));

		readbuf = (str_eglChooseConfig*)buf;

		if(configs != NULL) {
			config_length = sizeof(EGLConfig) * config_size;
			memcpy(configs, (buf + readbuf->configs_buf_offset), config_length);
		}
	}

	*num_config = readbuf->re_num_config;
	result = readbuf->retVal;

	if(buf != NULL)	
		free(buf);

	return result;
}
EGLint eglGetError(void)
{
	int Ret;
	str_eglGetError *writebuf;
	str_eglGetError *readbuf;

	writebuf = (str_eglGetError*)wbuf;
	readbuf = (str_eglGetError*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_eglGetError;
	writebuf->hd.retFlag = 1;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_eglGetError;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	EGLint retVal = readbuf->retVal;
	
	return retVal;
}

EGLSurface eglCreateWindowSurface(EGLDisplay display, EGLConfig config, NativeWindowType native_window, EGLint const *attrib_list)
{
	int Ret, i=0;
	str_eglCreateWindowSurface *writebuf;
	str_eglCreateWindowSurface *readbuf;
	EGLSurface eglsurface;

	writebuf = (str_eglCreateWindowSurface*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglCreateWindowSurface;
	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->display = display;
	// EGLconfig is a void pointer, but it is only used by gl functions
	writebuf->config = config;
	writebuf->window = native_window;

	writebuf->attrib_list[0] = EGL_NONE;
	if(attrib_list != NULL) {
		while(attrib_list[i]!=EGL_NONE)
			i++;
		writebuf->attrib_count = i + 1;	// include EGL_NONE
		memcpy(writebuf->attrib_list, attrib_list, (sizeof(EGLint)*writebuf->attrib_count));
	}

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglCreateWindowSurface*)rbuf;
	init_header(&(readbuf->hd));
	readbuf->hd.funcID = ID_eglCreateWindowSurface;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	
	eglsurface = readbuf->retVal;
	
	return eglsurface;
	
}

EGLContext eglCreateContext(EGLDisplay display, EGLConfig config, EGLContext share_context, EGLint const *attrib_list)
{
	int Ret, i=0;
	str_eglCreateContext *writebuf;
	str_eglCreateContext *readbuf;
	EGLContext eglcontext;

	writebuf = (str_eglCreateContext*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglCreateContext;
	writebuf->hd.retFlag = 1;

	writebuf->display = display;
	writebuf->config = config;
	// share_context means a pointer that indicates already created other context
	writebuf->share_context = share_context;

	writebuf->attrib_list[0] = EGL_NONE;
	if(attrib_list != NULL) {
		while(attrib_list[i]!=EGL_NONE)
			i++;
		writebuf->attrib_count = i + 1;	// include EGL_NONE
		memcpy(writebuf->attrib_list, attrib_list, (sizeof(EGLint)*writebuf->attrib_count));
	}

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglCreateContext*)rbuf;
	init_header(&(readbuf->hd));
	readbuf->hd.funcID = ID_eglCreateContext;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	
	eglcontext = readbuf->retVal;
	
	return eglcontext;
}

EGLBoolean eglMakeCurrent(EGLDisplay display, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
	int Ret;
	EGLBoolean result;
	str_eglMakeCurrent *writebuf;
	str_eglMakeCurrent *readbuf;

	writebuf = (str_eglMakeCurrent*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglMakeCurrent;
	writebuf->hd.retFlag = RT_REQUEST;
	
	writebuf->display = display;
	writebuf->draw = draw;
	writebuf->read = read;
	writebuf->ctx = ctx;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglMakeCurrent*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglMakeCurrent);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

const char * EGLAPIENTRY eglQueryString(EGLDisplay dpy, EGLint name)
{
	int Ret;
	str_eglQueryString *writebuf;
	str_eglQueryString *readbuf;

	writebuf = (str_eglQueryString*)wbuf;
	readbuf = (str_eglQueryString*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));
	
	writebuf->hd.funcID = ID_eglQueryString;
	writebuf->hd.retFlag = 1;

	writebuf->dpy = dpy;
	writebuf->name = name;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_eglQueryString;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	memset(ret_string, 0, sizeof(ret_string));
	if(readbuf->retVal[0] != '\n')
		strcpy(ret_string, readbuf->retVal);
	else
		return NULL;
	
	return ret_string;
}

EGLBoolean eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
{
	int Ret;
	EGLBoolean result;
	str_eglQuerySurface *writebuf;
	str_eglQuerySurface *readbuf;

	writebuf = (str_eglQuerySurface*)wbuf;
	readbuf = (str_eglQuerySurface*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));
	
	writebuf->hd.funcID = ID_eglQuerySurface;
	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->dpy = dpy;
	writebuf->surface = surface;
	writebuf->attribute = attribute;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_eglQuerySurface;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	if(value != NULL)
		*value = readbuf->re_value;
	result = readbuf->retVal;
	
	return result;
}

EGLBoolean eglSwapBuffers(EGLDisplay display, EGLSurface surface)
{
	int Ret;
	EGLBoolean result;
	str_eglSwapBuffers *writebuf;
	str_eglSwapBuffers *readbuf;

	writebuf = (str_eglSwapBuffers*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglSwapBuffers;
	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->display = display;
	writebuf->surface = surface;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglSwapBuffers*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglSwapBuffers);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}
EGLBoolean eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
	int Ret;
	EGLBoolean result;
	str_eglDestroyContext *writebuf;
	str_eglDestroyContext *readbuf;

	writebuf = (str_eglDestroyContext*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglDestroyContext;
	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->dpy = dpy;
	writebuf->ctx = ctx;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglDestroyContext*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglDestroyContext);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}
EGLBoolean eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
	int Ret;
	EGLBoolean result;
	str_eglDestroySurface *writebuf;
	str_eglDestroySurface *readbuf;
	
	writebuf = (str_eglDestroySurface*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = ID_eglDestroySurface;
	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->dpy = dpy;
	writebuf->surface = surface;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglDestroySurface*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglDestroySurface);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}
EGLBoolean eglTerminate(EGLDisplay dpy)
{
	int Ret;
	EGLBoolean result;
	str_eglTerminate *writebuf;
	str_eglTerminate *readbuf;

	writebuf = (str_eglTerminate*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglTerminate;
	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->dpy = dpy;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglTerminate*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglTerminate);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

// the last function
EGLBoolean eglReleaseThread(void)
{
	int Ret;
	EGLBoolean result;
	str_eglReleaseThread *writebuf;
	str_eglReleaseThread *readbuf;

	writebuf = (str_eglReleaseThread*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglReleaseThread;
	writebuf->hd.retFlag = RT_REQUEST;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglReleaseThread*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglReleaseThread);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	// close the device
	close(dev);

	free_unfreed_list(unfreed);

	return result;
}

EGLBoolean eglGetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
	int Ret;
	EGLBoolean result;
	void *buf = NULL;
	str_eglGetConfigs *writebuf;
	str_eglGetConfigs *readbuf;

	EGLint struct_length = sizeof(str_eglGetConfigs);
	EGLint config_length = 0;
	EGLint total_length = 0;

	writebuf = (str_eglGetConfigs*)wbuf;
	memset(writebuf, 0, struct_length);

	writebuf->hd.funcID = func_ID(eglGetConfigs);
	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->dpy = dpy;
	writebuf->config_size = config_size;
	if(configs == NULL)
		writebuf->configs_buf_offset = 0;
	else
		writebuf->configs_buf_offset = struct_length;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglGetConfigs*)rbuf;
	readbuf->hd.funcID = func_ID(eglGetConfigs);
	
	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	if(readbuf->hd.ldata_length > 0) {
		total_length = readbuf->hd.ldata_length + COMM_HEAD_SIZE;
		
		buf = (void*)malloc(total_length);
		memset(buf, 0, total_length);
		memcpy(buf, readbuf, COMM_HEAD_SIZE);

		Ret = read_without_overflow(dev,(buf + COMM_HEAD_SIZE),(readbuf->hd.ldata_length + MAX_BUF_SIZE));

		readbuf = (str_eglGetConfigs*)buf;

		if(configs != NULL) {
			config_length = sizeof(EGLConfig) * config_size;
			memcpy(configs, (buf + readbuf->configs_buf_offset), config_length);
		}
	}

	*num_config = readbuf->num_config;
	result = readbuf->retVal;

	if(buf != NULL)	
		free(buf);

	return result;
}

EGLBoolean eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
{
	int Ret;
	EGLBoolean result;
	str_eglGetConfigAttrib *writebuf;
	str_eglGetConfigAttrib *readbuf;

	writebuf = (str_eglGetConfigAttrib*)wbuf;
	readbuf = (str_eglGetConfigAttrib*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_eglGetConfigAttrib;
	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->config = config;
	writebuf->attribute = attribute;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_eglGetConfigAttrib;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	*value = readbuf->value;
	result = readbuf->retVal;

	return result;
}

EGLSurface eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
{
	int Ret;
	str_eglCreatePbufferSurface *writebuf;
	str_eglCreatePbufferSurface *readbuf;
	EGLSurface surface;

	writebuf = (str_eglCreatePbufferSurface*)wbuf;
	readbuf = (str_eglCreatePbufferSurface*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_eglCreatePbufferSurface;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->dpy = dpy;
	writebuf->config = config;

	memset((writebuf->attrib_list), 0, PARAM_SIZE(eglCreatePbufferSurface));
	writebuf->attrib_list[0] = EGL_NONE;
	if(attrib_list != NULL)
		memcpy((writebuf->attrib_list), attrib_list, PARAM_SIZE(eglCreatePbufferSurface));

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_eglCreatePbufferSurface;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	surface = readbuf->retVal;

	return surface;
}

EGLSurface eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list)
{
	int Ret;
	str_eglCreatePixmapSurface *writebuf;
	str_eglCreatePixmapSurface *readbuf;
	EGLSurface surface;

	writebuf = (str_eglCreatePixmapSurface*)wbuf;
	readbuf = (str_eglCreatePixmapSurface*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_eglCreatePixmapSurface;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->dpy = dpy;
	writebuf->config = config;
	writebuf->pixmap = pixmap;

	memset((writebuf->attrib_list), 0, PARAM_SIZE(eglCreatePixmapSurface));
	writebuf->attrib_list[0] = EGL_NONE;
	if(attrib_list != NULL)
		memcpy((writebuf->attrib_list), attrib_list, PARAM_SIZE(eglCreatePixmapSurface));

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_eglCreatePixmapSurface;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	surface = readbuf->retVal;

	return surface;
}

EGLenum eglQueryAPI(void)
{
	int Ret;
	str_eglQueryAPI *writebuf;
	str_eglQueryAPI *readbuf;
	EGLenum enum_value;

	writebuf = (str_eglQueryAPI*)wbuf;
	readbuf = (str_eglQueryAPI*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_eglQueryAPI;
	writebuf->hd.retFlag = RT_REQUEST;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	enum_value = readbuf->retVal;

	return enum_value;
}

EGLBoolean eglWaitClient(void)
{
	int Ret;
	EGLBoolean result;
	str_eglWaitClient *writebuf;
	str_eglWaitClient *readbuf;

	writebuf = (str_eglWaitClient*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglWaitClient;
	writebuf->hd.retFlag = RT_REQUEST;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglWaitClient*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglWaitClient);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

EGLSurface eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
{
	int Ret;
	str_eglCreatePbufferFromClientBuffer *writebuf;
	str_eglCreatePbufferFromClientBuffer *readbuf;
	EGLSurface surface;

	writebuf = (str_eglCreatePbufferFromClientBuffer*)wbuf;
	readbuf = (str_eglCreatePbufferFromClientBuffer*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_eglCreatePbufferFromClientBuffer;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->dpy = dpy;
	writebuf->buftype = buftype;
	writebuf->buffer = buffer;
	writebuf->config = config;

	memset((writebuf->attrib_list), 0, PARAM_SIZE(eglCreatePbufferFromClientBuffer));
	writebuf->attrib_list[0] = EGL_NONE;
	if(attrib_list != NULL)
		memcpy((writebuf->attrib_list), attrib_list, PARAM_SIZE(eglCreatePbufferFromClientBuffer));

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_eglCreatePbufferFromClientBuffer;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	surface = readbuf->retVal;

	return surface;
}

EGLBoolean eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
{
	int Ret;
	EGLBoolean result;
	str_eglSurfaceAttrib *writebuf;
	str_eglSurfaceAttrib *readbuf;

	writebuf = (str_eglSurfaceAttrib*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglSurfaceAttrib;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->dpy = dpy;
	writebuf->surface = surface;
	writebuf->attribute = attribute;
	writebuf->value = value;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglSurfaceAttrib*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglSurfaceAttrib);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

EGLBoolean eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	int Ret;
	EGLBoolean result;
	str_eglBindTexImage *writebuf;
	str_eglBindTexImage *readbuf;

	writebuf = (str_eglBindTexImage*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglBindTexImage;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->dpy = dpy;
	writebuf->surface = surface;
	writebuf->buffer = buffer;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglBindTexImage*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglBindTexImage);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

EGLBoolean eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	int Ret;
	EGLBoolean result;
	str_eglReleaseTexImage *writebuf;
	str_eglReleaseTexImage *readbuf;

	writebuf = (str_eglReleaseTexImage*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglReleaseTexImage;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->dpy = dpy;
	writebuf->surface = surface;
	writebuf->buffer = buffer;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglReleaseTexImage*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglReleaseTexImage);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

EGLBoolean eglSwapInterval(EGLDisplay dpy, EGLint interval)
{
	int Ret;
	EGLBoolean result;
	str_eglSwapInterval *writebuf;
	str_eglSwapInterval *readbuf;

	writebuf = (str_eglSwapInterval*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglSwapInterval;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->dpy = dpy;
	writebuf->interval = interval;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglSwapInterval*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglSwapInterval);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

EGLContext eglGetCurrentContext(void)
{
	int Ret;
	str_eglGetCurrentContext *writebuf;
	str_eglGetCurrentContext *readbuf;
	EGLContext context;

	writebuf = (str_eglGetCurrentContext*)wbuf;
	readbuf = (str_eglGetCurrentContext*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_eglGetCurrentContext;
	writebuf->hd.retFlag = RT_REQUEST;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_eglGetCurrentContext;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	context = readbuf->retVal;

	return context;
}

EGLSurface eglGetCurrentSurface(EGLint readdraw)
{
	int Ret;
	str_eglGetCurrentSurface *writebuf;
	str_eglGetCurrentSurface *readbuf;
	EGLSurface surface;

	writebuf = (str_eglGetCurrentSurface*)wbuf;
	readbuf = (str_eglGetCurrentSurface*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_eglGetCurrentSurface;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->readdraw = readdraw;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_eglGetCurrentSurface;
	
	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	surface = readbuf->retVal;

	return surface;
}

EGLDisplay eglGetCurrentDisplay(void)
{
	int Ret;
	str_eglGetCurrentDisplay *writebuf;
	str_eglGetCurrentDisplay *readbuf;
	EGLDisplay display;

	writebuf = (str_eglGetCurrentDisplay*)wbuf;
	readbuf = (str_eglGetCurrentDisplay*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_eglGetCurrentDisplay;
	writebuf->hd.retFlag = RT_REQUEST;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_eglGetCurrentDisplay;
	
	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	display = readbuf->retVal;

	return display;
}

EGLBoolean eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value)
{
	int Ret;
	EGLBoolean result;
	str_eglQueryContext *writebuf;
	str_eglQueryContext *readbuf;

	writebuf = (str_eglQueryContext*)wbuf;
	readbuf = (str_eglQueryContext*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_eglQueryContext;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->dpy = dpy;
	writebuf->ctx = ctx;
	writebuf->attribute = attribute;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_eglQueryContext;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	*value = readbuf->value;
	result = readbuf->retVal;

	return result;
}

EGLBoolean eglWaitGL(void)
{
	int Ret;
	EGLBoolean result;
	str_eglWaitGL *writebuf;
	str_eglWaitGL *readbuf;

	writebuf = (str_eglWaitGL*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglWaitGL;
	writebuf->hd.retFlag = RT_REQUEST;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglWaitGL*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglWaitGL);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

EGLBoolean eglWaitNative(EGLint engine)
{
	int Ret;
	EGLBoolean result;
	str_eglWaitNative *writebuf;
	str_eglWaitNative *readbuf;

	writebuf = (str_eglWaitNative*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglWaitNative;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->engine = engine;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglWaitNative*)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(eglWaitNative);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

EGLBoolean eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
{
	int Ret;
	EGLBoolean result;
	str_eglCopyBuffers *writebuf;
	str_eglCopyBuffers *readbuf;

	writebuf = (str_eglCopyBuffers*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_eglCopyBuffers;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->dpy = dpy;
	writebuf->surface = surface;
	writebuf->target = target;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf = (str_eglCopyBuffers*)rbuf;
	init_header(&(readbuf->hd));
	
	readbuf->hd.funcID = func_ID(eglCopyBuffers);

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

