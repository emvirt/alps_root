#include <stdio.h>
#include <dlfcn.h>

#include "EGL/halfdpy.h"

EGLint display_width = 0;
EGLint display_height = 0;
EGLint half_width = DEFAULT_HALF_WIDTH;
EGLint half_height = DEFAULT_HALF_HEIGHT;

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


/*********** For secure world **************/

// cyoung : Modfied to seperate a display ------------------
EGLNativeWindowType fbCreateWindow(EGLNativeDisplayType Display, int X, int Y, int Width, int Height)
{
	EGLNativeWindowType (*orig_fbCreateWindow)(EGLNativeDisplayType Display, int X, int Y, int Width, int Height) = NULL;
	EGLNativeWindowType native_window = (EGLNativeWindowType)0;

	orig_fbCreateWindow = hldpy_GetFuncAddr("libEGL.so", "fbCreateWindow");
	if(orig_fbCreateWindow == NULL)
	{
		printf("fbCreateWindow() is not hooked.\n");
		return 0;
	}

	// Get the width and height values of a Display
	fbGetDisplayGeometry(Display, &display_width, &display_height);

	half_width = 1920;
	half_height = 720;

//	half_width = display_width / 2;
//	half_height = display_height / 2;

	if(Height == 0 || Height > half_height)
	{
		// Divides the dispaly
		Width = half_width;
		Height = half_height;
	}

	// Create a window on the left half of the display
	native_window = orig_fbCreateWindow(Display, X, Y, Width, Height);

	return native_window;
}

void fbGetDisplayGeometry(EGLNativeDisplayType Display, int *Width, int *Height)
{
	void (*orig_fbGetDisplayGeometry)(EGLNativeDisplayType Display, int *Width, int *Height) = NULL;

	orig_fbGetDisplayGeometry = hldpy_GetFuncAddr("libEGL.so", "fbGetDisplayGeometry");
	if(orig_fbGetDisplayGeometry == NULL)
	{
		printf("fbGetDisplayGeometry() is not hooked.\n");
		return ;
	}

	orig_fbGetDisplayGeometry(Display, Width, Height);

	if(*Width > 1920)
		*Width = 1920;
	if(*Height > 720)
		*Height = 720;
}

