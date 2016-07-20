#include "EGL/egl.h"
#include "GLES2/gl2.h"

#define DEFAULT_DPY_WIDTH 1920
#define DEFAULT_DPY_HEIGHT 720
#define DEFAULT_HALF_WIDTH (DEFAULT_DPY_WIDTH / 2)
#define DEFAULT_HALF_HEIGHT (DEFAULT_DPY_HEIGHT / 2)

// cyoung : Get the address of the orinal function
void *hldpy_GetFuncAddr(char *objname, char *funcname);

// cyoung : Modfied to seperate a display ------------------
EGLNativeWindowType fbCreateWindow(EGLNativeDisplayType Display, int X, int Y, int Width, int Height);
void fbGetDisplayGeometry(EGLNativeDisplayType Display, int *Width, int *Height);
