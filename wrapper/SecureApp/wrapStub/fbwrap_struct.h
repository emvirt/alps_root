#include "wrapStub_define.h"

typedef struct struct_fbGetDisplayByInex{
	typ_common_hd hd;

	EGLNativeDisplayType retVal;	//ret val

	int DisplayIndex;				// parameter
} str_fbgetdpyidx;

typedef struct struct_fbCreateWidnow{
	typ_common_hd hd;

	EGLNativeWindowType retVal;	//ret val

	EGLNativeDisplayType display;	// parameters
	int x;
	int y;
	int width;
	int height;
} str_fbcrwin;

///////////////////////////////////
typedef struct struct_fbGetDisplay
{
	typ_common_hd hd;

	EGLNativeDisplayType retVal;
	
	char context[PARAM_SIZE_fbGetDisplay];
}str_fbGetDisplay;

typedef struct struct_fbGetDisplayGeometry
{
	typ_common_hd hd;

	EGLNativeDisplayType Display;
	int Width;
	int Height;
}str_fbGetDisplayGeometry;

typedef struct struct_fbGetDisplayInfo
{
	typ_common_hd hd;

	EGLNativeDisplayType Display;
	int Width;
	int Height;
	unsigned long Physical;
	int Stride;
	int BitsPerPixel;
}str_fbGetDisplayInfo;

typedef struct struct_fbDestroyDisplay
{
	typ_common_hd hd;

	EGLNativeDisplayType Display;
}str_fbDestroyDisplay;

typedef struct struct_fbGetWindowGeometry
{
	typ_common_hd hd;

	EGLNativeWindowType Window;
	int X;
	int Y;
	int Width;
	int Height;
}str_fbGetWindowGeometry;

typedef struct struct_fbGetWindowInfo
{
	typ_common_hd hd;

	EGLNativeWindowType Window;
	int X;
	int Y;
	int Width;
	int Height;
	int BitsPerPixel;
	unsigned int Offset;
}str_fbGetWindowInfo;

typedef struct struct_fbDestroyWindow
{
	typ_common_hd hd;

	EGLNativeWindowType Window;
}str_fbDestroyWindow;

typedef struct struct_fbCreatePixmap
{
	typ_common_hd hd;

	EGLNativePixmapType retVal;
	
	EGLNativeDisplayType Display;
	int Width;
	int Height;
}str_fbCreatePixmap;

typedef struct struct_fbCreatePixmapWithBpp
{
	typ_common_hd hd;

	EGLNativePixmapType retVal;

	EGLNativeDisplayType Display;
	int Width;
	int Height;
	int BitsPerPixel;
}str_fbCreatePixmapWithBpp;

typedef struct struct_fbGetPixmapGeometry
{
	typ_common_hd hd;

	EGLNativePixmapType Pixmap;
	int Width;
	int Height;
}str_fbGetPixmapGeometry;

typedef struct struct_fbGetPixmapInfo
{
	typ_common_hd hd;

	EGLNativePixmapType Pixmap;
	int Width;
	int Height;
	int BitsPerPixel;
	int Stride;
	char Bits[PARAM_SIZE_fbGetPixmapInfo];
}str_fbGetPixmapInfo;

typedef struct struct_fbDestroyPixmap
{
	typ_common_hd hd;

	EGLNativePixmapType Pixmap;
}str_fbDestroyPixmap;

