#include <GLES2/gl2.h>
#include <EGL/egl.h>

#define SECURE_WORLD
//#define WRAP_DEBUG

#define DEVICE_FILENAME	"/dev/mydev"

#define COMM_HEAD_ID_SIZE (sizeof(unsigned int))
#define COMM_HEAD_RETFLAG_SIZE (sizeof(unsigned int))
#define COMM_HEAD_LDATALEN_SIZE (sizeof(unsigned int))
#define COMM_HEAD_SIZE \
	(COMM_HEAD_ID_SIZE \
	+ COMM_HEAD_RETFLAG_SIZE \
	+ COMM_HEAD_LDATALEN_SIZE)

#define MAX_BUF_SIZE 512
#define MAX_LDATA_SIZE 0x4000U
#define MAX_LDATA_SIZE_1M 0x100000U
#define PARAM_SIZE(func) (PARAM_SIZE_##func)

#define MAX_VERTEX_ARR_SIZE 448	// 512 - 64 bytes
#define MAX_GL_BUFFER_SIZE 480		// 512 - 32 bytes

#define MAX_ATTRIB_COUNT 128		// maximum number of elements of an attib_list
#define MAX_ATTRIB_VALUE_COUNT MAX_ATTRIB_COUNT
#define MAX_ATTRIB_COUNT_WITH_VAL (MAX_ATTRIB_COUNT + MAX_ATTRIB_VALUE_COUNT)
#define MAX_CONFIG_COUNT 256		// maximum number of configs

#ifdef WRAP_DEBUG
#define call_log(retVal)	printf("%s is called !!!\n", __func__)
#else
#define call_log(retVal) ;
#endif

#define init_header(hd_addr) memset(hd_addr, 0, COMM_HEAD_SIZE);
#define typ_struct(func) (struct_##func)
#define func_ID(func) (ID_##func)

// ioctl commands
#define IMMEDIATE_SEND_START 0
#define IMMEDIATE_SEND_STOP 1

// common header for all functions
typedef struct struct_header {
	unsigned int funcID;
	unsigned int retFlag;
	unsigned int ldata_length;
//	unsigned int ldata_start_offset;
} typ_common_hd;

enum function_ID{
	ID_glEnable = 0,
	ID_glClear,
	ID_glClearColor,
	ID_glDisableVertexAttribArray,
	ID_glUniformMatrix4fv,
	ID_glEnableVertexAttribArray,
	ID_glDrawArrays,
	ID_glVertexAttribPointer,
	ID_glViewport,
	ID_glCreateShader,

	ID_glShaderSource,			//10
	ID_glCompileShader,
	ID_glGetShaderiv,
	ID_glGetShaderInfoLog,
	ID_glCreateProgram,
	ID_glAttachShader,
	ID_glBindAttribLocation,
	ID_glLinkProgram,
	ID_glGetProgramiv,
	ID_glGetProgramInfoLog,

	ID_glGetUniformLocation,	//20
	ID_glDeleteShader,
	ID_glUseProgram,
	ID_glActiveTexture,
	ID_glBindBuffer,
	ID_glBindFramebuffer,
	ID_glBindRenderbuffer,
	ID_glBindTexture,
	ID_glBlendColor,
	ID_glBlendEquation,

	ID_glBlendEquationSeparate,		//30
	ID_glBlendFunc,
	ID_glBlendFuncSeparate,
	ID_glBufferData,
	ID_glBufferSubData,
	ID_glCheckFramebufferStatus,
	ID_glClearDepthf,
	ID_glClearStencil,
	ID_glColorMask,
	ID_glCompressedTexImage2D,

	ID_glCompressedTexSubImage2D,	//40
	ID_glCopyTexImage2D,
	ID_glCopyTexSubImage2D,
	ID_glCullFace,
	ID_glDeleteBuffers,
	ID_glDeleteFramebuffers,
	ID_glDeleteProgram,
	ID_glDeleteRenderbuffers,
	ID_glDeleteTextures,
	ID_glDepthFunc,

	ID_glDepthMask,					//50
	ID_glDepthRangef,
	ID_glDetachShader,
	ID_glDisable,
	ID_glDrawElements,
	ID_glFinish,
	ID_glFlush,
	ID_glFramebufferRenderbuffer,
	ID_glFramebufferTexture2D,
	ID_glFrontFace,

	ID_glGenBuffers,					//60
	ID_glGenerateMipmap,
	ID_glGenFramebuffers,
	ID_glGenRenderbuffers,
	ID_glGenTextures,
	ID_glGetActiveAttrib,
	ID_glGetActiveUniform,
	ID_glGetAttachedShaders,
	ID_glGetAttribLocation,
	ID_glGetBooleanv,

	ID_glGetBufferParameteriv,		//70
	ID_glGetError,
	ID_glGetFloatv,
	ID_glGetFramebufferAttachmentParameteriv,
	ID_glGetIntegerv,
	ID_glGetRenderbufferParameteriv,
	ID_glGetShaderPrecisionFormat,
	ID_glGetShaderSource,
	ID_glGetString,
	ID_glGetTexParameterfv,

	ID_glGetTexParameteriv,			//80
	ID_glGetUniformfv,
	ID_glGetUniformiv,
	ID_glGetVertexAttribfv,
	ID_glGetVertexAttribiv,
	ID_glGetVertexAttribPointerv,
	ID_glHint,
	ID_glIsBuffer,
	ID_glIsEnabled,
	ID_glIsFramebuffer,

	ID_glIsProgram,					//90
	ID_glIsRenderbuffer,
	ID_glIsShader,
	ID_glIsTexture,
	ID_glLineWidth,
	ID_glPixelStorei,
	ID_glPolygonOffset,
	ID_glReadPixels,
	ID_glReleaseShaderCompiler,
	ID_glRenderbufferStorage,

	ID_glSampleCoverage,			//100
	ID_glScissor,
	ID_glShaderBinary,
	ID_glStencilFunc,
	ID_glStencilFuncSeparate,
	ID_glStencilMask,
	ID_glStencilMaskSeparate,
	ID_glStencilOp,
	ID_glStencilOpSeparate,
	ID_glTexImage2D,

	ID_glTexParameterf,				//110
	ID_glTexParameterfv,
	ID_glTexParameteri,
	ID_glTexParameteriv,
	ID_glTexSubImage2D,
	ID_glUniform1f,
	ID_glUniform1fv,
	ID_glUniform1i,
	ID_glUniform1iv,
	ID_glUniform2f,

	ID_glUniform2fv,				//120
	ID_glUniform2i,
	ID_glUniform2iv,
	ID_glUniform3f,
	ID_glUniform3fv,
	ID_glUniform3i,
	ID_glUniform3iv,
	ID_glUniform4f,
	ID_glUniform4fv,
	ID_glUniform4i,

	ID_glUniform4iv,				//130
	ID_glUniformMatrix2fv,
	ID_glUniformMatrix3fv,
	ID_glValidateProgram,
	ID_glVertexAttrib1f,
	ID_glVertexAttrib1fv,
	ID_glVertexAttrib2f,
	ID_glVertexAttrib2fv,
	ID_glVertexAttrib3f,
	ID_glVertexAttrib3fv,

	ID_glVertexAttrib4f,			//140
	ID_glVertexAttrib4fv,
	ID_eglGetDisplay,			// egl ID start 
	ID_eglInitialize,
	ID_eglBindAPI,
	ID_eglChooseConfig,
	ID_eglGetError,
	ID_eglCreateWindowSurface,
	ID_eglCreateContext,
	ID_eglMakeCurrent,

	ID_eglDestroyContext,			//150
	ID_eglDestroySurface,
	ID_eglTerminate,
	ID_eglReleaseThread,
	ID_eglQueryString,
	ID_eglQuerySurface,
	ID_eglSwapBuffers,
	ID_eglGetConfigs,
	ID_eglGetConfigAttrib,
	ID_eglCreatePbufferSurface,

	ID_eglCreatePixmapSurface,		//160
	ID_eglQueryAPI,
	ID_eglWaitClient,
	ID_eglCreatePbufferFromClientBuffer,
	ID_eglSurfaceAttrib,
	ID_eglBindTexImage,
	ID_eglReleaseTexImage,
	ID_eglSwapInterval,
	ID_eglGetCurrentContext,
	ID_eglGetCurrentSurface,

	ID_eglGetCurrentDisplay,			//170
	ID_eglQueryContext,
	ID_eglWaitGL,
	ID_eglWaitNative,
	ID_eglCopyBuffers,
	ID_fbGetDisplayByIndex,		//vivante ID start
	ID_fbCreateWindow,
	ID_fbGetDisplay,
	ID_fbGetDisplayGeometry,
	ID_fbGetDisplayInfo,

	ID_fbDestroyDisplay,				//180
	ID_fbGetWindowGeometry,
	ID_fbGetWindowInfo,
	ID_fbDestroyWindow,
	ID_fbCreatePixmap,
	ID_fbCreatePixmapWithBpp,
	ID_fbGetPixmapGeometry,
	ID_fbGetPixmapInfo,
	ID_fbDestroyPixmap				//188
};

enum return_type {
	RT_NONE = 0,
	RT_REQUEST,
	RT_RESPONSE
};

/* For vivante functions */

#define PARAM_SIZE_fbGetDisplay \
	(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
					+ sizeof(EGLNativeDisplayType)))

#define PARAM_SIZE_fbGetPixmapInfo \
	(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
					+ sizeof(EGLNativePixmapType) \
					+ sizeof(int) \
					+ sizeof(int) \
					+ sizeof(int) \
					+ sizeof(int)))

/* For EGL */

#define PARAM_SIZE_eglChooseConfig \
	(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
					+ sizeof(EGLBoolean) \
					+ sizeof(EGLDisplay) \
					+ sizeof(EGLint) \
					+ sizeof(EGLint) \
					+ sizeof(EGLint)))

#define PARAM_SIZE_eglCreateWindowSurface \
	(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
					+ sizeof(EGLSurface) \
					+ sizeof(EGLDisplay) \
					+ sizeof(EGLConfig) \
					+ sizeof(NativeWindowType) \
					+ sizeof(EGLint)))

#define PARAM_SIZE_eglCreateContext \
	(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
					+ sizeof(EGLDisplay) \
					+ sizeof(EGLConfig) \
					+ sizeof(EGLContext) \
					+ sizeof(EGLContext) \
					+ sizeof(EGLint)))

#define PARAM_SIZE_eglCreatePbufferSurface \
	(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
					+ sizeof(EGLSurface) \
					+ sizeof(EGLDisplay) \
					+ sizeof(EGLConfig)))

#define PARAM_SIZE_eglCreatePbufferFromClientBuffer \
	(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
					+ sizeof(EGLSurface) \
					+ sizeof(EGLDisplay) \
					+ sizeof(EGLenum) \
					+ sizeof(EGLClientBuffer) \
					+ sizeof(EGLConfig)))

#define PARAM_SIZE_eglCreatePixmapSurface \
	(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
					+ sizeof(EGLSurface) \
					+ sizeof(EGLDisplay) \
					+ sizeof(EGLConfig) \
					+ sizeof(EGLNativePixmapType)))

/* For GLESv2 */

#define PARAM_SIZE_glVertexAttribPointer \
	(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
					+ sizeof(GLuint) \
					+ sizeof(GLint) \
					+ sizeof(GLenum) \
					+ sizeof(GLboolean) \
					+ sizeof(GLsizei)))

#define PARAM_SIZE_glShaderSource \
	(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
					+ sizeof(GLuint) \
					+ sizeof(GLsizei) \
					+ sizeof(GLuint) \
					+ sizeof(GLuint)))

#define PARAM_SIZE_glGetShaderInfoLog \
		(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
						+ sizeof(GLuint) \
						+ sizeof(GLsizei) \
						+ sizeof(GLsizei)))

#define PARAM_SIZE_glBindAttribLocation \
		(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
						+ sizeof(GLuint) \
						+ sizeof(GLuint)))

#define PARAM_SIZE_glGetProgramInfoLog \
		(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
						+ sizeof(GLuint) \
						+ sizeof(GLsizei) \
						+ sizeof(GLsizei)))

#define PARAM_SIZE_glGetUniformLocation \
			(MAX_BUF_SIZE - (COMM_HEAD_SIZE \
							+ sizeof(GLuint) \
							+ sizeof(GLint)))

// a list of memory spaces created by malloc
struct space_list {
	void *buf;
	struct space_list *next;
};
