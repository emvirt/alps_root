#include "wrapStub_define.h"

/*********** For EGL library ***************/

typedef struct struct_eglGetDisplay{
	typ_common_hd hd;

	EGLDisplay retVal; //ret val
	EGLNativeDisplayType param_eglNativeDisplayType;	//param 
}str_eglGetDisplay;

typedef struct struct_eglInitialize{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay display;
	EGLint major;
	EGLint minor;
}str_eglInitialize;

typedef struct struct_eglGetError{
	typ_common_hd hd;

	EGLint retVal;
}str_eglGetError;
typedef struct strcut_eglBindAPI{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLenum api;
}str_eglBindAPI;

typedef struct struct_eglChooseConfig{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay display;
	// EGLconfig is a void pointer, but it is only used by gl functions
	EGLint configs_buf_offset;
	EGLint config_size;
	union {
		EGLint re_num_config;
		EGLint attrib_count;
	};
	EGLint attrib_list[PARAM_SIZE_eglChooseConfig/sizeof(EGLint)];
}str_eglChooseConfig;

typedef struct struct_eglCreateWindowSurface{
	typ_common_hd hd;

	EGLSurface retVal;

	EGLDisplay display;
	EGLConfig config;
	NativeWindowType window;
	EGLint attrib_count;
	EGLint attrib_list[PARAM_SIZE_eglCreateWindowSurface/sizeof(EGLint)];
}str_eglCreateWindowSurface;

typedef struct struct_eglCreateContext{
	typ_common_hd hd;

	EGLContext retVal;

	EGLDisplay display;
	EGLConfig config;
	EGLContext share_context;
	EGLint attrib_count;
	EGLint attrib_list[PARAM_SIZE_eglCreateContext/sizeof(EGLint)];
}str_eglCreateContext;

typedef struct struct_eglMakeCurrent{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay display;
	EGLSurface draw;
	EGLSurface read;
	EGLContext ctx;
}str_eglMakeCurrent;

typedef struct struct_eglQueryString{
	typ_common_hd hd;

	EGLDisplay dpy;
	EGLint name;

	char retVal[128];
}str_eglQueryString;

typedef struct struct_eglQuerySurface{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
	EGLSurface surface;
	EGLint attribute;
	EGLint re_value;
}str_eglQuerySurface;

typedef struct struct_eglSwapBuffers{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay display;
	EGLSurface surface;
}str_eglSwapBuffers;

typedef struct struct_eglDestroyContext{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
	EGLContext ctx;
}str_eglDestroyContext;

typedef struct struct_eglDestroySurface{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
	EGLContext surface;
}str_eglDestroySurface;

typedef struct struct_eglTerminate{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
}str_eglTerminate;

typedef struct struct_eglReleaseThread{
	typ_common_hd hd;

	EGLBoolean retVal;
}str_eglReleaseThread;

typedef struct struct_eglGetConfigs
{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
	EGLint configs_buf_offset;
	EGLint config_size;
	EGLint num_config;
}str_eglGetConfigs;

typedef struct struct_eglGetConfigAttrib
{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
	EGLConfig config;
	EGLint attribute;
	EGLint value;
}str_eglGetConfigAttrib;

typedef struct struct_eglCreatePbufferSurface
{
	typ_common_hd hd;

	EGLSurface retVal;
	
	EGLDisplay dpy;
	EGLConfig config;
	EGLint attrib_list[PARAM_SIZE_eglCreatePbufferSurface/sizeof(EGLint)];
}str_eglCreatePbufferSurface;

typedef struct struct_eglCreatePixmapSurface
{
	typ_common_hd hd;

	EGLSurface retVal;

	EGLDisplay dpy;
	EGLConfig config;
	EGLNativePixmapType pixmap;
	EGLint attrib_list[PARAM_SIZE_eglCreatePixmapSurface/sizeof(EGLint)];
}str_eglCreatePixmapSurface;

typedef struct struct_eglQueryAPI
{
	typ_common_hd hd;

	EGLenum retVal;
}str_eglQueryAPI;

typedef struct struct_eglWaitClient
{
	typ_common_hd hd;

	EGLBoolean retVal;
}str_eglWaitClient;

typedef struct struct_eglCreatePbufferFromClientBuffer
{
	typ_common_hd hd;

	EGLSurface retVal;

	EGLDisplay dpy;
	EGLenum buftype;
	EGLClientBuffer buffer;
	EGLConfig config;
	EGLint attrib_list[PARAM_SIZE_eglCreatePbufferFromClientBuffer/sizeof(EGLint)];
}str_eglCreatePbufferFromClientBuffer;

typedef struct struct_eglSurfaceAttrib
{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
	EGLSurface surface;
	EGLint attribute;
	EGLint value;
}str_eglSurfaceAttrib;

typedef struct struct_eglBindTexImage
{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
	EGLSurface surface;
	EGLint buffer;
}str_eglBindTexImage;

typedef struct struct_eglReleaseTexImage
{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
	EGLSurface surface;
	EGLint buffer;
}str_eglReleaseTexImage;

typedef struct struct_eglSwapInterval
{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
	EGLint interval;
}str_eglSwapInterval;

typedef struct struct_eglGetCurrentContext
{
	typ_common_hd hd;

	EGLContext retVal;
}str_eglGetCurrentContext;

typedef struct struct_eglGetCurrentSurface
{
	typ_common_hd hd;

	EGLSurface retVal;

	EGLint readdraw;
}str_eglGetCurrentSurface;

typedef struct struct_eglGetCurrentDisplay
{
	typ_common_hd hd;

	EGLDisplay retVal;
}str_eglGetCurrentDisplay;

typedef struct struct_eglQueryContext
{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
	EGLContext ctx;
	EGLint attribute;
	EGLint value;
}str_eglQueryContext;

typedef struct struct_eglWaitGL
{
	typ_common_hd hd;

	EGLBoolean retVal;
}str_eglWaitGL;

typedef struct struct_eglWaitNative
{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLint engine;
}str_eglWaitNative;

typedef struct struct_eglCopyBuffers
{
	typ_common_hd hd;

	EGLBoolean retVal;

	EGLDisplay dpy;
	EGLSurface surface;
	EGLNativePixmapType target;
}str_eglCopyBuffers;

