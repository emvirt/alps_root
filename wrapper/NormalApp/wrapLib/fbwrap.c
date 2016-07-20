#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "fbwrap_struct.h"

extern int read_from_shmem(int filp, void *buf, int count);

extern int dev;

extern char wbuf[MAX_BUF_SIZE];
extern char rbuf[MAX_BUF_SIZE];

//initialize function
EGLNativeDisplayType fbGetDisplayByIndex(int DisplayIndex)
{
	int ret;
	str_fbgetdpyidx *writebuf;
	str_fbgetdpyidx *readbuf;
	EGLNativeDisplayType ret_eglNativeDisplayType;

	writebuf = (str_fbgetdpyidx*)wbuf;
	readbuf = (str_fbgetdpyidx*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_fbGetDisplayByIndex;
	writebuf->hd.retFlag = 1;
	writebuf->DisplayIndex = DisplayIndex;
	
	dev = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);
	printf("device open. dev = %d\n", dev);
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_fbGetDisplayByIndex;

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	ret_eglNativeDisplayType = readbuf->retVal;

	return ret_eglNativeDisplayType;
}

EGLNativeWindowType fbCreateWindow(EGLNativeDisplayType Display, int X, int Y, int Width, int Height)
{
	int ret;
	str_fbcrwin *writebuf;
	str_fbcrwin *readbuf;
	EGLNativeWindowType ret_nativedpy;

	writebuf = (str_fbcrwin*)wbuf;
	readbuf = (str_fbcrwin*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_fbCreateWindow;
	writebuf->hd.retFlag = 1;
	
	writebuf->display = Display;
	writebuf->x = X;
	writebuf->y = Y;
	writebuf->width = Width;
	writebuf->height = Height;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_fbCreateWindow;

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	ret_nativedpy = readbuf->retVal;
	
	return ret_nativedpy;
}

////////////////////////////////////////////////////
EGLNativeDisplayType fbGetDisplay(void *context)
{
	int ret;
	str_fbGetDisplay *writebuf;
	str_fbGetDisplay *readbuf;
	EGLNativeDisplayType ret_nativedpy;

	writebuf = (str_fbGetDisplay*)wbuf;
	readbuf = (str_fbGetDisplay*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_fbGetDisplay;
	writebuf->hd.retFlag = RT_REQUEST;

	memset(writebuf->context, 0, PARAM_SIZE(fbGetDisplay));
	if(context != NULL)
		memcpy(writebuf->context, context, PARAM_SIZE(fbGetDisplay));

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_fbGetDisplay;

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	ret_nativedpy = readbuf->retVal;
	
	return ret_nativedpy;
}

void fbGetDisplayGeometry(EGLNativeDisplayType Display, int * Width, int * Height)
{
	int ret;
	str_fbGetDisplayGeometry *writebuf;
	str_fbGetDisplayGeometry *readbuf;

	writebuf = (str_fbGetDisplayGeometry*)wbuf;
	readbuf = (str_fbGetDisplayGeometry*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_fbGetDisplayGeometry;
	writebuf->hd.retFlag = RT_REQUEST;
	
	writebuf->Display = Display;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_fbGetDisplayGeometry;

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	*Width = readbuf->Width;
	*Height = readbuf->Height;
}

void fbGetDisplayInfo(EGLNativeDisplayType Display, int * Width, int * Height, unsigned long * Physical, int * Stride, int * BitsPerPixel)
{
	int ret;
	str_fbGetDisplayInfo *writebuf;
	str_fbGetDisplayInfo *readbuf;

	writebuf = (str_fbGetDisplayInfo*)wbuf;
	readbuf = (str_fbGetDisplayInfo*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_fbGetDisplayInfo;
	writebuf->hd.retFlag = RT_REQUEST;
	
	writebuf->Display = Display;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_fbGetDisplayInfo;

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	*Width = readbuf->Width;
	*Height = readbuf->Height;
	*Physical = readbuf->Physical;
	*Stride = readbuf->Stride;
	*BitsPerPixel = readbuf->BitsPerPixel;
}

void fbDestroyDisplay(EGLNativeDisplayType Display)
{
	int ret;
	str_fbDestroyDisplay *writebuf;

	writebuf = (str_fbDestroyDisplay*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_fbDestroyDisplay;
	writebuf->hd.retFlag = RT_NONE;
	
	writebuf->Display = Display;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();
}

void fbGetWindowGeometry(EGLNativeWindowType Window, int * X, int * Y, int * Width, int * Height)
{
	int ret;
	str_fbGetWindowGeometry *writebuf;
	str_fbGetWindowGeometry *readbuf;

	writebuf = (str_fbGetWindowGeometry*)wbuf;
	readbuf = (str_fbGetWindowGeometry*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_fbGetWindowGeometry;
	writebuf->hd.retFlag = RT_REQUEST;
	
	writebuf->Window = Window;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_fbGetWindowGeometry;

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	*X = readbuf->X;
	*Y = readbuf->Y;
	*Width = readbuf->Width;
	*Height = readbuf->Height;
}

void fbGetWindowInfo(EGLNativeWindowType Window, int * X, int * Y, int * Width, int * Height, int * BitsPerPixel, unsigned int * Offset)
{
	int ret;
	str_fbGetWindowInfo *writebuf;
	str_fbGetWindowInfo *readbuf;

	writebuf = (str_fbGetWindowInfo*)wbuf;
	readbuf = (str_fbGetWindowInfo*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_fbGetWindowInfo;
	writebuf->hd.retFlag = RT_REQUEST;
	
	writebuf->Window = Window;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_fbGetWindowInfo;

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	*X = readbuf->X;
	*Y = readbuf->Y;
	*Width = readbuf->Width;
	*Height = readbuf->Height;
	*BitsPerPixel = readbuf->BitsPerPixel;
	*Offset = readbuf->Offset;
}

void fbDestroyWindow(EGLNativeWindowType Window)
{
	int ret;
	str_fbDestroyWindow *writebuf;

	writebuf = (str_fbDestroyWindow*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_fbDestroyWindow;
	writebuf->hd.retFlag = RT_NONE;
	
	writebuf->Window = Window;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();
}

EGLNativePixmapType fbCreatePixmap(EGLNativeDisplayType Display, int Width, int Height)
{
	int ret;
	str_fbCreatePixmap *writebuf;
	str_fbCreatePixmap *readbuf;
	EGLNativePixmapType ret_pixmap;

	writebuf = (str_fbCreatePixmap*)wbuf;
	readbuf = (str_fbCreatePixmap*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_fbCreatePixmap;
	writebuf->hd.retFlag = RT_REQUEST;
	
	writebuf->Display = Display;
	writebuf->Width = Width;
	writebuf->Height = Height;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_fbCreatePixmap;

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	ret_pixmap = readbuf->retVal;
	
	return ret_pixmap;
}

EGLNativePixmapType fbCreatePixmapWithBpp(EGLNativeDisplayType Display, int Width, int Height, int BitsPerPixel)
{
	int ret;
	str_fbCreatePixmapWithBpp *writebuf;
	str_fbCreatePixmapWithBpp *readbuf;
	EGLNativePixmapType ret_pixmap;

	writebuf = (str_fbCreatePixmapWithBpp*)wbuf;
	readbuf = (str_fbCreatePixmapWithBpp*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_fbCreatePixmapWithBpp;
	writebuf->hd.retFlag = RT_REQUEST;
	
	writebuf->Display = Display;
	writebuf->Width = Width;
	writebuf->Height = Height;
	writebuf->BitsPerPixel = BitsPerPixel;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_fbCreatePixmapWithBpp;

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	ret_pixmap = readbuf->retVal;
	
	return ret_pixmap;
}

void fbGetPixmapGeometry(EGLNativePixmapType Pixmap,int * Width,int * Height)
{
	int ret;
	str_fbGetPixmapGeometry *writebuf;
	str_fbGetPixmapGeometry *readbuf;

	writebuf = (str_fbGetPixmapGeometry*)wbuf;
	readbuf = (str_fbGetPixmapGeometry*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_fbGetPixmapGeometry;
	writebuf->hd.retFlag = RT_REQUEST;
	
	writebuf->Pixmap = Pixmap;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_fbGetPixmapGeometry;

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	*Width = readbuf->Width;
	*Height = readbuf->Height;
}

void fbGetPixmapInfo(EGLNativePixmapType Pixmap, int * Width, int * Height, int * BitsPerPixel, int * Stride, void ** Bits)
{
	int ret;
	str_fbGetPixmapInfo *writebuf;
	str_fbGetPixmapInfo *readbuf;

	writebuf = (str_fbGetPixmapInfo*)wbuf;
	readbuf = (str_fbGetPixmapInfo*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_fbGetPixmapInfo;
	writebuf->hd.retFlag = RT_REQUEST;
	
	writebuf->Pixmap = Pixmap;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();

	readbuf->hd.funcID = ID_fbGetPixmapInfo;

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	*Width = readbuf->Width;
	*Height = readbuf->Height;
	*BitsPerPixel = readbuf->BitsPerPixel;
	*Stride = readbuf->Stride;
	if(*Bits != NULL) {
		memcpy(*Bits, readbuf->Bits, PARAM_SIZE(fbGetPixmapInfo));
	}
}

void fbDestroyPixmap(EGLNativePixmapType Pixmap)
{
	int ret;
	str_fbDestroyPixmap *writebuf;

	writebuf = (str_fbDestroyPixmap*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_fbDestroyPixmap;
	writebuf->hd.retFlag = RT_REQUEST;
	
	writebuf->Pixmap = Pixmap;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log();
}

