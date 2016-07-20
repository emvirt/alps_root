#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "wrapStub_struct.h"
#include "functions.h"

#include "func_pointers.h"

#define MAX_INT_COUNT (MAX_BUF_SIZE / sizeof(int))

int dev = -1;

int main(void){
	int funcID, quit=1;
	int ret;
	int pCopybuf[MAX_INT_COUNT] ={0, }; //shared memory buf

	dev = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);
	if(dev < 0){
		printf("dev open error\nmake dev file (mknod /dev/mydev c 240 32) or insmod module (insmod ipi.ko)\n");
		printf("open error value : %d\n", dev);
		return 1;
	}
	printf("device open. dev = %d\n", dev);

	//infinite repeat
	do{
		ret = -1;
		
		while(ret <= 0) {
			ret = read(dev, pCopybuf, MAX_BUF_SIZE);	//copy from shared mem
			if(ret > 0) break;

			usleep(1000);
		}

		//compare ID and find function
		funcID = *pCopybuf;
#ifdef WRAP_DEBUG
		printf("function ID: %d\n", funcID);
#endif

		wrap_func_ptr[funcID](pCopybuf);

/*switch (funcID) {
			case ID_glEnable :
				call_glEnable(pDatabuf);
				break;

			case ID_glClear:
				call_glClear(pDatabuf);
				break;

			case ID_glClearColor:
				call_glClearColor(pDatabuf);
				break;

			case ID_glDisableVertexAttribArray:
				call_glDisableVertexAttribArray(pDatabuf);
				break;

			case ID_glUniformMatrix4fv:
				call_glUniformMatrix4fv(pDatabuf);
				break;

			case ID_glEnableVertexAttribArray:
				call_glEnableVertexAttribArray(pDatabuf);
				break;

			case ID_glDrawArrays:
				call_glDrawArrays(pDatabuf);
				break;

			case ID_glVertexAttribPointer:
				call_glVertexAttribPointer(pDatabuf);
				break;

			case ID_glViewport:
				call_glViewport(pDatabuf);
				break;

			case ID_glCreateShader:
				call_glCreateShader(pDatabuf);
				break;

			case ID_glShaderSource:
				call_glShaderSource(pDatabuf);
				break;

			case ID_glCompileShader:
				call_glCompileShader(pDatabuf);
				break;

			case ID_glGetShaderiv:
				call_glGetShaderiv(pDatabuf);
				break;

			case ID_glGetShaderInfoLog:
				call_glGetShaderInfoLog(pDatabuf);
				break;

			case ID_glCreateProgram:
				call_glCreateProgram(pDatabuf);
				break;
			case ID_glAttachShader:
				call_glAttachShader(pDatabuf);
				break;
			case ID_glBindAttribLocation:
				call_glBindAttribLocation(pDatabuf);
				break;

			case ID_glLinkProgram:
				call_glLinkProgram(pDatabuf);
				break;

			case ID_glGetProgramiv:
				call_glGetProgramiv(pDatabuf);
				break;

			case ID_glGetProgramInfoLog:
				call_glGetProgramInfoLog(pDatabuf);
				break;

			case ID_glGetUniformLocation:
				call_glGetUniformLocation(pDatabuf);
				break;

			case ID_glDeleteShader:
				call_glDeleteShader(pDatabuf);
				break;

			case ID_glUseProgram:
				call_glUseProgram(pDatabuf);
				break;

			case ID_eglGetDisplay:
				call_eglGetDisplay(pDatabuf);
				break;

			case ID_eglInitialize:
				call_eglInitialize(pDatabuf);
				break;

			case ID_eglBindAPI:
				call_eglBindAPI(pDatabuf);
				break;

			case ID_eglChooseConfig:
				call_eglChooseConfig(pDatabuf);
				break;

			case ID_eglGetError:
				call_eglGetError(pDatabuf);
				break;

			case ID_eglCreateWindowSurface:
				call_eglCreateWindowSurface(pDatabuf);
				break;

			case ID_eglCreateContext:
				call_eglCreateContext(pDatabuf);
				break;

			case ID_eglMakeCurrent:
				call_eglMakeCurrent(pDatabuf);
				break;

			case ID_eglDestroyContext:
				call_eglDestroyContext(pDatabuf);
				break;

			case ID_eglDestroySurface:
				call_eglDestroySurface(pDatabuf);
				break;

			case ID_eglTerminate:
				call_eglTerminate(pDatabuf);
				break;

			case ID_eglReleaseThread:
				call_eglReleaseThread(pDatabuf);
				break;

			case ID_eglQueryString:
				call_eglQueryString(pDatabuf);
				break;
				
			case ID_eglQuerySurface:
				call_eglQuerySurface(pDatabuf);
				break;

			case ID_eglSwapBuffers:
				call_eglSwapBuffers(pDatabuf);
				break;

			case ID_fbGetDisplayByIndex:
				call_fbGetDisplayByIndex(pDatabuf);
				break;

			case ID_fbCreateWindow:
				call_fbCreateWindow(pDatabuf);
				break;

			default:
				printf("unknown function ID\n");
				break;
		}
*/
	}while(quit);

	if(dev != -1)
		close (dev);
	
	return 0;
}