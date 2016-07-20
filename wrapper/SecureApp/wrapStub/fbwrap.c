#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>

#include "fbwrap_struct.h"
//#include "EGL/halfdpy.h" 
#include "functions.h"

extern int dev;

// cyoung : Get the address of the orinal function
void *hldpy_GetFuncAddr(char *objname, char *funcname)
{
        void *handle = NULL;
        void *func = NULL;

        if(objname == NULL || funcname == NULL)
                return NULL;

        // open a binary file with the original function
        handle = dlopen(objname, RTLD_LAZY);
        if(handle == NULL)
        {
                printf("dlopen() failed : %s\n", dlerror());
                return NULL;
        }

        // search the address using a function name
        func = dlsym(handle, funcname);
        if(func == NULL)
		{
				printf("dlsym() failed : %s\n", dlerror());
				dlclose(handle);
				return NULL;
		}

		dlclose(handle);

		return func;
}

/********** Modified fbCreateWindow() for normal world ***********/
EGLNativeWindowType fbCreateWindow_normal(EGLNativeDisplayType Display, int X, int Y, int Width, int Height)
{
	EGLNativeWindowType (*orig_fbCreateWindow)(EGLNativeDisplayType Display, int X, int Y, int Width, int Height);
	EGLNativeWindowType native_window = (EGLNativeWindowType)0;
	int normal_x = X;
	int normal_y = Y;
	int normal_width = Width;
	int normal_height = Height;

	orig_fbCreateWindow = hldpy_GetFuncAddr("libEGL.so", "fbCreateWindow");

	// Display size and start position for normal world are statically fixed 
//	normal_width = 1920;
//	normal_height = 360;
//	normal_x = 0;
//	normal_y = 720;

//	normal_width = 1920;
//	normal_height = 1080;
//	normal_x = 0;
//	normal_y = 0;

	// Create a window on the left half of the display
	native_window = orig_fbCreateWindow(Display, normal_x, normal_y, normal_width, normal_height);

	return native_window;
}


/* EGL functions */
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

int call_fbGetDisplayByIndex(int *pDatabuf)
{
	str_fbgetdpyidx *function;
	int display_index;
	int ret;
	
	function = (str_fbgetdpyidx *)pDatabuf;
	display_index = function->DisplayIndex;
	
	function->retVal = fbGetDisplayByIndex(display_index);

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_fbCreateWindow(int *pDatabuf)
{
	str_fbcrwin *function;
	int ret;
	
	function = (str_fbcrwin *)pDatabuf;

	// For normal window
	if(function->width > 1920)
		function->width = 1920;
	if(function->height > 360)
		function->height = 360;

	function->x = 0;
	function->y = 720;

	function->retVal = fbCreateWindow_normal(function->display, function->x, function->y, function->width, function->height);
//	function->retVal = fbCreateWindow(function->display, function->x, function->y, function->width, function->height);

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_fbGetDisplay(int *pDatabuf)
{
	str_fbGetDisplay *function;
	EGLNativeDisplayType native;
	void *context = NULL;
	unsigned int context_front = 0;
	int ret;
	
	function = (str_fbGetDisplay *)pDatabuf;

	context_front = *((int*)(function->context));
	if(context_front == 0x0)
		context = NULL;
	else
		context = function->context;

	native = fbGetDisplay(context);

	function->retVal = native;

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}


int call_fbGetDisplayGeometry(int *pDatabuf)
{
	str_fbGetDisplayGeometry *function;
	int width, height;
	int ret;
	
	function = (str_fbGetDisplayGeometry *)pDatabuf;

	fbGetDisplayGeometry(function->Display, &width, &height);

	// For normal window
	if(width > 1920)
		width = 1920;
	if(height > 360)
		height = 360;

	function->Width = width;
	function->Height = height;

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_fbGetDisplayInfo(int *pDatabuf)
{
	str_fbGetDisplayInfo *function;
	int width=0, height=0, stride=0, bitperpixel=0;
	unsigned long physical=0;
	int ret;
	
	function = (str_fbGetDisplayInfo *)pDatabuf;

	fbGetDisplayInfo(function->Display, &width, &height, &physical, &stride, &bitperpixel);

	// For normal window
/*	if(width > 1920)
		width = 1920;
	if(height > 360)
		height = 360;
*/
	function->Width = width;
	function->Height = height;
	function->Physical = physical;
	function->Stride = stride;
	function->BitsPerPixel = bitperpixel;

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_fbDestroyDisplay(int *pDatabuf)
{
	str_fbDestroyDisplay *function;
	
	function = (str_fbDestroyDisplay *)pDatabuf;

	fbDestroyDisplay(function->Display);

	return 0;
}

int call_fbGetWindowGeometry(int *pDatabuf)
{
	str_fbGetWindowGeometry *function;
	int width, height, x, y;
	int ret;
	
	function = (str_fbGetWindowGeometry *)pDatabuf;

	fbGetWindowGeometry(function->Window, &x, &y, &width, &height);

	function->Width = width;
	function->Height = height;
	function->X = x;
	function->Y = y;

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_fbGetWindowInfo(int *pDatabuf)
{
	str_fbGetWindowInfo *function;
	int width=0, height=0, x=0, y=0, bitsperpixel=0;
	unsigned int offset=0;
	int ret;

	function = (str_fbGetWindowInfo *)pDatabuf;

	fbGetWindowInfo(function->Window, &x, &y, &width, &height, &bitsperpixel, &offset);

	function->Width = width;
	function->Height = height;
	function->X = x;
	function->Y = y;
	function->BitsPerPixel = bitsperpixel;
	function->Offset = offset;

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_fbDestroyWindow(int *pDatabuf)
{
	str_fbDestroyWindow *function;
	
	function = (str_fbDestroyWindow *)pDatabuf;

	fbDestroyWindow(function->Window);

	return 0;
}

int call_fbCreatePixmap(int *pDatabuf)
{
	str_fbCreatePixmap *function;
	EGLNativePixmapType ret_pixmap;
	int ret;

	function = (str_fbCreatePixmap *)pDatabuf;

	ret_pixmap = fbCreatePixmap(function->Display, function->Width, function->Height);

	function->retVal = ret_pixmap;
	
	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_fbCreatePixmapWithBpp(int *pDatabuf)
{
	str_fbCreatePixmapWithBpp *function;
	EGLNativePixmapType ret_pixmap;
	int ret;

	function = (str_fbCreatePixmapWithBpp *)pDatabuf;

	ret_pixmap = fbCreatePixmapWithBpp(function->Display, function->Width, function->Height, function->BitsPerPixel);

	function->retVal = ret_pixmap;
	
	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_fbGetPixmapGeometry(int *pDatabuf)
{
	str_fbGetPixmapGeometry *function;
	int width, height;
	int ret;
	
	function = (str_fbGetPixmapGeometry *)pDatabuf;

	fbGetPixmapGeometry(function->Pixmap, &width, &height);

	function->Width = width;
	function->Height = height;

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_fbGetPixmapInfo(int *pDatabuf)
{
	str_fbGetPixmapInfo *function;
	int width, height, bitsperpixel, stride;
	void *bits = NULL;
	int ret;
	
	function = (str_fbGetPixmapInfo *)pDatabuf;

	fbGetPixmapInfo(function->Pixmap, &width, &height, &bitsperpixel, &stride, &bits);

	function->Width = width;
	function->Height = height;
	function->BitsPerPixel = bitsperpixel;
	function->Stride = stride;
	
	memset(function->Bits, 0, PARAM_SIZE(fbGetPixmapInfo));
	if(bits != NULL)
		memcpy(function->Bits, bits, PARAM_SIZE(fbGetPixmapInfo));
	
	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_fbDestroyPixmap(int *pDatabuf)
{
	str_fbDestroyPixmap *function;
	
	function = (str_fbDestroyPixmap *)pDatabuf;

	fbDestroyPixmap(function->Pixmap);

	return 0;
}

