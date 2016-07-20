#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <assert.h>

#include "eglwrap_struct.h"
#include "functions.h"

#define eglerror(x) \
	do { \
		x = eglGetError();\
		if(x != EGL_SUCCESS){\
			printf("eglGetError() = 0x%x\n", x);\
			assert(x == EGL_SUCCESS);\
		}\
	} while(0)

extern int read_without_overflow(int fp, void *buf, int count);
extern int dev;

/********** EGL functions ***********/
/*
 * Shape of all functions is as follow: 
 *         return_type call_$(original gl function name) (int *pDatabuf) { ... }
 * Parameters:
 *         pDatabuf : pointer that indicate received data,
 *                         included function id, return request, and parameters
 * The body of each function is based on follows:
 *         1) identify the received parameters
 *         2) copy parameters to local variable if it is required
 *         3) call original GL function
 *         4) write return values to the shared memory between worlds
 */
 
int call_eglGetDisplay(int *pDatabuf)
{
	int ret;
	str_eglGetDisplay *function;
	EGLint err;
	
	function = (str_eglGetDisplay *)pDatabuf;
	function->retVal = eglGetDisplay(function->param_eglNativeDisplayType);
	eglerror(err);

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglInitialize(int *pDatabuf)
{
	int ret;
	str_eglInitialize *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglInitialize *)pDatabuf;
	result = eglInitialize(function->display, &(function->major), &(function->minor));
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}


int call_eglBindAPI(int *pDatabuf)
{
	int ret;
	str_eglBindAPI *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglBindAPI *)pDatabuf;
	result = eglBindAPI(function->api);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglChooseConfig(int *pDatabuf)
{
	int ret;
	str_eglChooseConfig *function;
	str_eglChooseConfig *return_buf;
	EGLint *attrib_list = NULL;
	EGLint err;
	EGLint num_configs = 0;
	EGLConfig configs[MAX_CONFIG_COUNT] = {0, };
	EGLBoolean result;

	void *buf = NULL;
	EGLint attrib_length = 0;
	EGLint struct_length = sizeof(str_eglChooseConfig);
	EGLint config_length = 0;
	EGLint total_length = 0;
	
	function = (str_eglChooseConfig *)pDatabuf;

	/* 
	 * Parameter : attrib_list
	 *      If attrib_list has only EGL_NONE, it means NULL
	 *      Otherwise, this function copies received attrib_list data to a local buffer
	 */
	if(function->attrib_list[0] == EGL_NONE)
		attrib_list = NULL;
	else {
		attrib_length = sizeof(EGLint) * function->attrib_count;
		attrib_list = (EGLint*)malloc(attrib_length);
		memcpy(attrib_list, function->attrib_list, attrib_length);
	}

	/*
	 * If configs_buf_offset is bigger than zero, 
	 * it means that the user application requests coping the config data to a buffer
	 */
	if(function->configs_buf_offset > 0)
		result = eglChooseConfig(function->display, attrib_list, configs, function->config_size, &num_configs);
	else
		result = eglChooseConfig(function->display, attrib_list, NULL, function->config_size, &num_configs);
	eglerror(err);

	config_length = sizeof(EGLConfig) * function->config_size;
	total_length = struct_length + config_length;

	// Buffer to store return values and function information
	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);
	
	return_buf = (str_eglChooseConfig*)buf;
	return_buf->hd.funcID = function->hd.funcID;
	return_buf->hd.retFlag = RT_RESPONSE;
	return_buf->config_size = function->config_size;
	return_buf->re_num_config = num_configs;
	return_buf->retVal = result;

	/*
	 * When the user application requests coping the config data,
	 * the function write config data to the return buffer
	 */
	if(function->configs_buf_offset > 0) {
		return_buf->configs_buf_offset = struct_length;
		return_buf->hd.ldata_length = total_length - COMM_HEAD_SIZE;
		memcpy((buf + return_buf->configs_buf_offset), configs, config_length);
		ret = write(dev, buf, (total_length + MAX_BUF_SIZE));
	}
	else {
		return_buf->configs_buf_offset = 0;
		return_buf->hd.ldata_length = 0;
		ret = write(dev, return_buf, struct_length);
	}

	free(buf);
	if(attrib_list != NULL)
		free(attrib_list);

	return 0;
}

int call_eglGetError(int *pDatabuf)
{
	int ret;
	str_eglGetError *function;
	
	function = (str_eglGetError *)pDatabuf;
	function->retVal = eglGetError();

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglCreateWindowSurface(int *pDatabuf)
{
	int ret;
	str_eglCreateWindowSurface *function;
	EGLint *attrib_list = NULL;
	EGLint attrib_length = 0;
	EGLint err;

	function = (str_eglCreateWindowSurface *)pDatabuf;

	/* 
	 * Parameter : attrib_list
	 *      If attrib_list has only EGL_NONE, it means NULL
	 *      Otherwise, this function copies received attrib_list data to a local buffer
	 */
	if(function->attrib_list[0] == EGL_NONE)
		attrib_list = NULL;
	else {
		attrib_length = sizeof(EGLint) * function->attrib_count;
		attrib_list = (EGLint*)malloc(attrib_length);
		memcpy(attrib_list, function->attrib_list, attrib_length);
	}

	function->retVal = eglCreateWindowSurface(function->display, function->config, function->window, attrib_list);
	eglerror(err);

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	if(attrib_list != NULL)
		free(attrib_list);

	return 0;
}


int call_eglCreateContext(int *pDatabuf)
{
	int ret;
	str_eglCreateContext *function;
	EGLint *attrib_list = NULL;
	EGLint attrib_length = 0;
	EGLint err;

	function = (str_eglCreateContext *)pDatabuf;

	/* 
	 * Parameter : attrib_list
	 *      If attrib_list has only EGL_NONE, it means NULL
	 *      Otherwise, this function copies received attrib_list data to a local buffer
	 */
	if(function->attrib_list[0] == EGL_NONE)
		attrib_list = NULL;
	else {
		attrib_length = sizeof(EGLint) * function->attrib_count;
		attrib_list = (EGLint*)malloc(attrib_length);
		memcpy(attrib_list, function->attrib_list, attrib_length);
	}

	function->retVal = eglCreateContext(function->display, function->config, function->share_context, attrib_list);
	eglerror(err);

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	if(attrib_list != NULL)
		free(attrib_list);

	return 0;
}


int call_eglMakeCurrent(int *pDatabuf)
{
	int ret;
	str_eglMakeCurrent *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglMakeCurrent *)pDatabuf;

	result = eglMakeCurrent(function->display, function->draw, function->read, function->ctx);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}


int call_eglDestroyContext(int *pDatabuf)
{
	int ret;
	str_eglDestroyContext *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglDestroyContext *)pDatabuf;

	result = eglDestroyContext(function->dpy, function->ctx);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}


int call_eglDestroySurface(int *pDatabuf)
{
	int ret;
	str_eglDestroySurface *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglDestroySurface *)pDatabuf;

	result = eglDestroySurface(function->dpy, function->surface);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}


int call_eglTerminate (int *pDatabuf)
{
	int ret;
	str_eglTerminate *function;
	EGLint err;
	EGLBoolean result;

	function = (str_eglTerminate *)pDatabuf;

	result = eglTerminate(function->dpy);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

/* 
 * The last function.
 * Most of gl applications call eglReleaseThread when they are terminated.
 * So, we close the device for communication between worlds in this function.
 */
int call_eglReleaseThread(int *pDatabuf)
{
	int ret;
	str_eglReleaseThread *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglReleaseThread *)pDatabuf;

	result = eglReleaseThread();
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	// close and reset the device pointer for world commnunication
	close(dev);
	dev = -1;

	return 0;
}

/*
EGL_CLIENT_APIS
OpenGL, OpenGL_ES, or OpenVG. These strings correspond respectively to values EGL_OPENGL_API, EGL_OPENGL_ES_API, and EGL_OPENVG_API of the eglBindAPI, api argument.

EGL_VENDOR
Returns the company responsible for this EGL implementation. This name does not change from release to release.

EGL_VERSION
Returns a version or release number. The EGL_VERSION string is laid out as follows:

major_version.minor_version space vendor_specific_info

EGL_EXTENSIONS
Returns a space separated list of supported extensions to EGL.
*/
int call_eglQueryString(int *pDatabuf)
{
	int ret;
	str_eglQueryString *function;
	const char *ret_string = NULL;
	EGLDisplay display;
	EGLint name;
	EGLint err;
	
	function = (str_eglQueryString *)pDatabuf;

	display = function->dpy;
	name = function->name;

	ret_string = eglQueryString(display, name);
	eglerror(err);

	memset(function->retVal, 0, 128);
	if(ret_string != NULL)
		strcpy(function->retVal, ret_string);

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}


int call_eglQuerySurface(int *pDatabuf)
{
	int ret;
	str_eglQuerySurface *function;
	EGLint err;
	EGLint value = 0;
	EGLBoolean result;
	
	function = (str_eglQuerySurface *)pDatabuf;

	result = eglQuerySurface(function->dpy, function->surface, function->attribute, &value);
	eglerror(err);

	function->re_value = value;
	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}


int call_eglSwapBuffers(int *pDatabuf)
{
	int ret;
	str_eglSwapBuffers *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglSwapBuffers *)pDatabuf;
	result = eglSwapBuffers(function->display, function->surface);

	// Second call to refresh old frames
	result = eglSwapBuffers(function->display, function->surface);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

//////////////////////////////////////////////////
int call_eglGetConfigs(int *pDatabuf)
{
	int ret;
	str_eglGetConfigs *function;
	str_eglGetConfigs *return_buf;
	EGLint err;
	EGLint num_configs = 0;
	EGLConfig configs[MAX_CONFIG_COUNT] = {0, };
	EGLBoolean result;

	void *buf = NULL;
	EGLint struct_length = sizeof(str_eglGetConfigs);
	EGLint config_length = 0;
	EGLint total_length = 0;
	
	function = (str_eglGetConfigs *)pDatabuf;

	if(function->configs_buf_offset > 0)
		result = eglGetConfigs(function->dpy, configs, function->config_size, &num_configs);
	else
		result = eglGetConfigs(function->dpy, NULL, function->config_size, &num_configs);
	eglerror(err);

	config_length = sizeof(EGLConfig) * function->config_size;
	total_length = struct_length + config_length;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);
	
	return_buf = (str_eglGetConfigs*)buf;
	return_buf->hd.funcID = function->hd.funcID;
	return_buf->hd.retFlag = RT_RESPONSE;
	return_buf->config_size = function->config_size;
	return_buf->num_config = num_configs;
	return_buf->retVal = result;

	if(function->configs_buf_offset > 0) {
		return_buf->configs_buf_offset = struct_length;
		return_buf->hd.ldata_length = total_length - COMM_HEAD_SIZE;
		memcpy((buf + return_buf->configs_buf_offset), configs, config_length);
		ret = write(dev, buf, (total_length + MAX_BUF_SIZE));
	}
	else {
		return_buf->configs_buf_offset = 0;
		return_buf->hd.ldata_length = 0;
		ret = write(dev, return_buf, struct_length);
	}

	if(buf != NULL)
		free(buf);

	return 0;
}

int call_eglGetConfigAttrib(int *pDatabuf)
{
	int ret;
	str_eglGetConfigAttrib *function;
	EGLint err;
	EGLint value = 0;
	EGLBoolean result;
	
	function = (str_eglGetConfigAttrib *)pDatabuf;

	result = eglGetConfigAttrib(function->dpy, function->config, function->attribute, &value);
	eglerror(err);

	function->value = value;
	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglCreatePbufferSurface(int *pDatabuf)
{
	int ret;
	str_eglCreatePbufferSurface *function;
	EGLint err;
	EGLSurface surface;
	EGLint *attrib_list = NULL;
	
	function = (str_eglCreatePbufferSurface *)pDatabuf;

	if(function->attrib_list[0] == EGL_NONE)
		attrib_list = NULL;
	else
		attrib_list = function->attrib_list;
	
	surface = eglCreatePbufferSurface(function->dpy, function->config, attrib_list);
	eglerror(err);

	function->retVal = surface;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglCreatePixmapSurface(int *pDatabuf)
{
	int ret;
	str_eglCreatePixmapSurface *function;
	EGLint err;
	EGLSurface surface;
	EGLint *attrib_list = NULL;
	
	function = (str_eglCreatePixmapSurface *)pDatabuf;

	if(function->attrib_list[0] == EGL_NONE)
		attrib_list = NULL;
	else
		attrib_list = function->attrib_list;
	
	surface = eglCreatePixmapSurface(function->dpy, function->config, function->pixmap, attrib_list);
	eglerror(err);

	function->retVal = surface;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglQueryAPI(int *pDatabuf)
{
	int ret;
	str_eglQueryAPI *function;
	EGLint err;
	EGLenum enum_value;
	
	function = (str_eglQueryAPI *)pDatabuf;

	enum_value = eglQueryAPI();
	eglerror(err);

	function->retVal = enum_value;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglWaitClient(int *pDatabuf)
{
	int ret;
	str_eglWaitClient *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglWaitClient *)pDatabuf;

	result = eglWaitClient();
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglCreatePbufferFromClientBuffer(int *pDatabuf)
{
	int ret;
	str_eglCreatePbufferFromClientBuffer *function;
	EGLint err;
	EGLSurface surface;
	EGLint *attrib_list = NULL;
	
	function = (str_eglCreatePbufferFromClientBuffer *)pDatabuf;

	if(function->attrib_list[0] == EGL_NONE)
		attrib_list = NULL;
	else
		attrib_list = function->attrib_list;
	
	surface = eglCreatePbufferFromClientBuffer(function->dpy, function->buftype, function->buffer, function->config, attrib_list);
	eglerror(err);

	function->retVal = surface;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglSurfaceAttrib(int *pDatabuf)
{
	int ret;
	str_eglSurfaceAttrib *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglSurfaceAttrib *)pDatabuf;

	result = eglSurfaceAttrib(function->dpy, function->surface, function->attribute, function->value);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglBindTexImage(int *pDatabuf)
{
	int ret;
	str_eglBindTexImage *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglBindTexImage *)pDatabuf;

	result = eglBindTexImage(function->dpy, function->surface, function->buffer);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglReleaseTexImage(int *pDatabuf)
{
	int ret;
	str_eglReleaseTexImage *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglReleaseTexImage *)pDatabuf;

	result = eglReleaseTexImage(function->dpy, function->surface, function->buffer);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglSwapInterval(int *pDatabuf)
{
	int ret;
	str_eglSwapInterval *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglSwapInterval *)pDatabuf;

	result = eglSwapInterval(function->dpy, function->interval);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglGetCurrentContext(int *pDatabuf)
{
	int ret;
	str_eglGetCurrentContext *function;
	EGLint err;
	EGLContext context;
	
	function = (str_eglGetCurrentContext *)pDatabuf;

	context = eglGetCurrentContext();
	eglerror(err);

	function->retVal = context;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglGetCurrentSurface(int *pDatabuf)
{
	int ret;
	str_eglGetCurrentSurface *function;
	EGLint err;
	EGLSurface surface;
	
	function = (str_eglGetCurrentSurface *)pDatabuf;

	surface = eglGetCurrentSurface(function->readdraw);
	eglerror(err);

	function->retVal = surface;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglGetCurrentDisplay(int *pDatabuf)
{
	int ret;
	str_eglGetCurrentDisplay *function;
	EGLint err;
	EGLDisplay display;
	
	function = (str_eglGetCurrentDisplay *)pDatabuf;

	display = eglGetCurrentDisplay();
	eglerror(err);

	function->retVal = display;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglQueryContext(int *pDatabuf)
{
	int ret;
	str_eglQueryContext *function;
	EGLint err;
	EGLint value = 0;
	EGLBoolean result;
	
	function = (str_eglQueryContext *)pDatabuf;

	result = eglQueryContext(function->dpy, function->ctx, function->attribute, &value);
	eglerror(err);

	function->value = value;
	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}
	
	return 0;
}

int call_eglWaitGL(int *pDatabuf)
{
	int ret;
	str_eglWaitGL *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglWaitGL *)pDatabuf;

	result = eglWaitGL();
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglWaitNative(int *pDatabuf)
{
	int ret;
	str_eglWaitNative *function;
	EGLint err;
	EGLBoolean result;
	
	function = (str_eglWaitNative *)pDatabuf;

	result = eglWaitNative(function->engine);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_eglCopyBuffers(int *pDatabuf)
{
	int ret;
	str_eglCopyBuffers *function;
	EGLint err;
	EGLBoolean result;

	function = (str_eglCopyBuffers *)pDatabuf;

	result = eglCopyBuffers(function->dpy, function->surface, function->target);
	eglerror(err);

	function->retVal = result;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

