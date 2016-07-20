#include <stdio.h>
#include <dlfcn.h>

#include "EGL/egl.h"
#include "GLES2/gl2.h"
//#include "EGL/eglvivante.h"
//#include "hookegl.h"

#define DEFAULT_DPY_WIDTH 1920
#define DEFAULT_DPY_HEIGHT 1080
#define DEFAULT_HALF_WIDTH (DEFAULT_DPY_WIDTH / 2)
#define DEFAULT_HALF_HEIGHT (DEFAULT_DPY_HEIGHT / 2)

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

	half_width = display_width / 2;
	half_height = display_height / 2;

	if(Width == 0 || Width > half_width)
	{
		// Divides the dispaly
		Width = half_width;
		Height = display_height;
	}

#ifdef NORMAL_WORLD
	if(X < Width)
	{
		X = X + half_width;
	}
#endif

	// Create a window on the left half of the display
	native_window = orig_fbCreateWindow(Display, X, Y, Width, Height);

	return native_window;
}

void hldpy_ModResolution(EGLint *new_attrib_list, const EGLint *attrib_list, EGLint width, EGLint height)
{
	int i = 0;
	EGLint attr = 0;
	EGLint value = 0;
	EGLint pixel = 0;

	if(new_attrib_list == NULL || attrib_list == NULL)
		return ;

	for(i=0; attrib_list[i] != EGL_NONE; i+=2)
	{
		new_attrib_list[i] = attrib_list[i];
		new_attrib_list[i] = attrib_list[i+1];

		attr = new_attrib_list[i];
		value = new_attrib_list[i+1];

		switch(attr)
		{
		case EGL_MAX_PBUFFER_PIXELS:
			new_attrib_list[i] = value / 4;
			pixel = width * height;
			if(new_attrib_list[i] > pixel)
				new_attrib_list[i] = pixel;
			break;
		case EGL_MAX_PBUFFER_WIDTH:
		case EGL_WIDTH:
			new_attrib_list[i] = value / 2;
			if(new_attrib_list[i] > width)
				new_attrib_list[i] = width;
			break;
		case EGL_MAX_PBUFFER_HEIGHT:
		case EGL_HEIGHT:
			new_attrib_list[i] = value / 2;
			if(new_attrib_list[i] > height)
				new_attrib_list[i] = height;
			break;
		}
	}
}

EGLSurface eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
{
	EGLSurface (*orig_eglCreateWindowSurface)(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list) = NULL;
	EGLSurface surface;

	EGLint new_attrib[128];
	EGLint *ptr_attrib = NULL;

	// Get the address of the original function
	orig_eglCreateWindowSurface = hldpy_GetFuncAddr("libEGL.so", "eglCreateWindowSurface");
	if(orig_eglCreateWindowSurface == NULL)
	{
		printf("eglCreateWindowSurface() is not hooked.\n");
		return 0;
	}

	// Reduce the width of a suface
	if(attrib_list != NULL)
	{
		hldpy_ModResolution(new_attrib, attrib_list, half_width, display_height);
		ptr_attrib = new_attrib;
	}

	// Call the original function
	surface = orig_eglCreateWindowSurface(dpy, config, win, ptr_attrib);

	return surface;
}

EGLSurface eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
{
	EGLSurface (*orig_eglCreatePbufferSurface)(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list) = NULL;
	EGLSurface surface;

	EGLint new_attrib[128];
	EGLint *ptr_attrib = NULL;

	// Get the address of the original function
	orig_eglCreatePbufferSurface = hldpy_GetFuncAddr("libEGL.so", "eglCreatePbufferSurfac");
	if(orig_eglCreatePbufferSurface == NULL)
	{
		printf("elgCreatePbufferSurface() is not hooked.\n");
		return 0;
	}

	// Reduce the width of a suface
	if(attrib_list != NULL)
	{
		hldpy_ModResolution(new_attrib, attrib_list, half_width, display_height);
		ptr_attrib = new_attrib;
	}

	// Call the original function
	surface = orig_eglCreatePbufferSurface(dpy, config, ptr_attrib);

	return surface;
}

EGLSurface eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list)
{
	EGLSurface (*orig_eglCreatePixmapSurface)(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list) = NULL;
	EGLSurface surface;

	EGLint new_attrib[128];
	EGLint *ptr_attrib = NULL;

	// Get the address of the original function
	orig_eglCreatePixmapSurface = hldpy_GetFuncAddr("libEGL.so", "eglCreatePixmapSurface");
	if(orig_eglCreatePixmapSurface == NULL)
	{
		printf("elgCreatePixmapSurface() is not hooked.\n");
		return 0;
	}

	// Reduce the width of a suface
	if(attrib_list != NULL)
	{
		hldpy_ModResolution(new_attrib, attrib_list, half_width, display_height);
		ptr_attrib = new_attrib;
	}

	// Call the original function
	surface = orig_eglCreatePixmapSurface(dpy, config, pixmap, ptr_attrib);

	return surface;
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	void (*orig_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height) = NULL;

	orig_glViewport = hldpy_GetFuncAddr("libGLESv2.so", "glViewport");
	if(orig_glViewport == NULL)
	{
		printf("elgCreatePixmapSurface() is not hooked.\n");
		return ;
	}

	// Reduce the resolution
	width = width / 2;
	height = height / 2;

	if(width == 0 || width > half_width)
		width = half_width;
	if(height == 0 || height > half_height)
		height = half_height;

	// Call the original function
	orig_glViewport(x, y, width, height);
}
