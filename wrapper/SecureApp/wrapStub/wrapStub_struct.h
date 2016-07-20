#include "wrapStub_define.h"

/*********** For GLES library ***************/

typedef struct struct_glCreateShader{
	typ_common_hd hd;

	GLuint retVal;

	GLenum shaderType;
}str_glCreateShader;

//#define STRING_ARR_SIZE (PARAM_SIZE_glShaderSource / 2)
//#define LENGTH_ARR_SIZE ((PARAM_SIZE_glShaderSource - STRING_ARR_SIZE) / sizeof(GLint))
#define LENGTH_ARR_COUNT 128
typedef struct struct_glShaderSource{
	typ_common_hd hd;
	
	GLuint shader;
	GLsizei count;
	GLuint len_buf_offset;
	GLuint str_buf_offset;
}str_glShaderSource;

typedef struct struct_glCompileShader
{
	typ_common_hd hd;

	GLuint shader;
}str_glCompileShader;

typedef struct struct_glGetShaderiv{
	typ_common_hd hd;

	GLuint shader;
	GLenum pname;
	GLint re_params;
}str_glGetShaderiv;

typedef struct struct_glGetShaderInfoLog{
	typ_common_hd hd;

	GLuint shader;
	GLsizei maxLength;
	GLsizei re_length;
	GLchar re_infoLog[PARAM_SIZE_glGetShaderInfoLog/sizeof(GLchar)];
}str_glGetShaderInfoLog;

typedef struct struct_glCreateProgram{
	typ_common_hd hd;

	GLuint retVal;
}str_glCreateProgram;

typedef struct strcut_glAttachShader{
	typ_common_hd hd;

	GLuint program;
	GLuint shader;
}str_glAttachShader;

typedef struct struct_glBindAttribLocation{
	typ_common_hd hd;

	GLuint program;
	GLuint index;
	GLchar name[PARAM_SIZE_glBindAttribLocation/sizeof(GLchar)];

}str_glBindAttribLocation;
	
typedef struct struct_glLinkProgram{
	typ_common_hd hd;
	
	GLuint program;
}str_glLinkProgram;

typedef struct struct_glGetProgramiv{
	typ_common_hd hd;

	GLuint program;
	GLenum pname;
	GLint re_params;
}str_glGetProgramiv;

typedef struct struct_glGetProgramInfoLog{
	typ_common_hd hd;

	GLuint program;
	GLsizei maxLength;
	GLsizei re_length;
	GLchar re_infoLog[PARAM_SIZE_glGetProgramInfoLog/sizeof(GLchar)];
}str_glGetProgramInfoLog;
	

typedef struct struct_glGetUniformLocation{
	typ_common_hd hd;

	GLint retVal;

	GLuint program;
	GLint size;
	GLint name_buf_offset;
}str_glGetUniformLocation;

typedef struct struct_glDeleteShader{
	typ_common_hd hd;

	GLuint shader;
}str_glDeleteShader;

typedef struct struct_glUseProgram{
	typ_common_hd hd;

	GLuint program;
}str_glUseProgram;

typedef struct struct_glClearColor{
	typ_common_hd hd;

	GLclampf red;
	GLclampf green;
	GLclampf blue;
	GLclampf alpha;
}str_glClearColor;

typedef struct struct_glEnable{
	typ_common_hd hd;

	GLenum cap;
}str_glEnable;

typedef struct struct_glClear{
	typ_common_hd hd;

	GLbitfield mask;
}str_glClear;

typedef struct struct_glViewport{
	typ_common_hd hd;

	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
}str_glViewport;

// "4" in the function name means a 4x4 matrix (16 values)
typedef struct struct_glUniformMatrix4fv{
	typ_common_hd hd;
	
	GLint location;
	GLsizei count;
	GLboolean transpose;
	GLfloat value[16];
}str_glUniformMatrix4fv;

typedef struct struct_glVertexAttribPointer{
	typ_common_hd hd;

	GLuint indx;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;

	// not allowed the void array type
	char ptr[PARAM_SIZE_glVertexAttribPointer/sizeof(char)];

}str_glVertexAttribPointer;

typedef struct struct_glEnableVertexAttribArray{
	typ_common_hd hd;

	GLuint index;
}str_glEnableVertexAttribArray;

typedef struct struct_glDrawArrays{
	typ_common_hd hd;

	GLenum mode;
	GLint first;
	GLsizei count;
}str_glDrawArrays;

typedef struct struct_glDisableVertexAttribArray{
	typ_common_hd hd;

	GLuint index;
}str_glDisableVertexAttribArray;

//////////////
typedef struct struct_glActiveTexture {
	typ_common_hd hd;
	
	GLenum texture;
}struct_glActiveTexture;

typedef struct struct_glBindBuffer {
	typ_common_hd hd;
	
	GLenum target;
	GLuint buffer;
}struct_glBindBuffer;

typedef struct struct_glBindFramebuffer {
	typ_common_hd hd;
	
	GLenum target;
	GLuint framebuffer;
}struct_glBindFramebuffer;
		
typedef struct struct_glBindRenderbuffer {
	typ_common_hd hd;
	
	GLenum target;
	GLuint renderbuffer;
}struct_glBindRenderbuffer;

typedef struct struct_glBindTexture {
	typ_common_hd hd;

	GLenum target;
	GLuint texture;
}struct_glBindTexture;

typedef struct struct_glBlendColor {
	typ_common_hd hd;

	GLclampf red;
	GLclampf green;
	GLclampf blue;
	GLclampf alpha;
}struct_glBlendColor;

typedef struct struct_glBlendEquation {
	typ_common_hd hd;
	
	GLenum mode;
}struct_glBlendEquation;

typedef struct struct_glBlendEquationSeparate {
	typ_common_hd hd;
	
	GLenum modeRGB;
	GLenum modeAlpha;
}struct_glBlendEquationSeparate;

typedef struct struct_glBlendFunc {
	typ_common_hd hd;
	
	GLenum sfactor;
	GLenum dfactor;
}struct_glBlendFunc;

typedef struct struct_glBlendFuncSeparate {
	typ_common_hd hd;
	
	GLenum srcRGB;
	GLenum dstRGB;
	GLenum srcAlpha;
	GLenum dstAlpha;
}struct_glBlendFuncSeparate;

typedef struct struct_glBufferData {
	typ_common_hd hd;
	
	GLenum target;
	GLsizeiptr size;
	GLint data_buf_offset;
	GLenum usage;
}struct_glBufferData;

typedef struct struct_glBufferSubData {
	typ_common_hd hd;
	
	GLenum target;
	GLintptr offset;
	GLsizeiptr size;
	GLint data_buf_offset;
}struct_glBufferSubData;

typedef struct struct_glCheckFramebufferStatus {
	typ_common_hd hd;
	
	GLenum retVal;
	
	GLenum target;
}struct_glCheckFramebufferStatus;

typedef struct struct_glClearDepthf {
	typ_common_hd hd;
	
	GLclampf depth;
}struct_glClearDepthf;

typedef struct struct_glClearStencil {
	typ_common_hd hd;
	
	GLint s;
}struct_glClearStencil;

typedef struct struct_glColorMask {
	typ_common_hd hd;
	
	GLboolean red;
	GLboolean green;
	GLboolean blue;
	GLboolean alpha;
}struct_glColorMask;

typedef struct struct_glCompressedTexImage2D {
	typ_common_hd hd;
	
	GLenum target;
	GLint level;
	GLenum internalformat;
	GLsizei width;
	GLsizei height;
	GLint border;
	GLsizei imageSize;
	GLint data_buf_offset;
}struct_glCompressedTexImage2D;

typedef struct struct_glCompressedTexSubImage2D {
	typ_common_hd hd;
	
	GLenum target;
	GLint level;
	GLint xoffset;
	GLint yoffset;
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLsizei imageSize;
	GLint data_buf_offset;
}struct_glCompressedTexSubImage2D;

typedef struct struct_glCopyTexImage2D {
	typ_common_hd hd;
	
	GLenum target;
	GLint level;
	GLenum internalformat;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	GLint border;
}struct_glCopyTexImage2D;

typedef struct struct_glCopyTexSubImage2D {
	typ_common_hd hd;
	
	GLenum target;
	GLint level;
	GLint xoffset;
	GLint yoffset;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
}struct_glCopyTexSubImage2D;

typedef struct struct_glCullFace {
	typ_common_hd hd;
	
	GLenum mode;
}struct_glCullFace;

typedef struct struct_glDeleteBuffers {
	typ_common_hd hd;
	
	GLsizei n;
	GLuint buffers[MAX_GL_BUFFER_SIZE/sizeof(GLuint)];
}struct_glDeleteBuffers;

typedef struct struct_glDeleteFramebuffers {
	typ_common_hd hd;
	
	GLsizei n;
	GLuint framebuffers[MAX_GL_BUFFER_SIZE/sizeof(GLuint)];
}struct_glDeleteFramebuffers;

typedef struct struct_glDeleteProgram {
	typ_common_hd hd;
	
	GLuint program;
}struct_glDeleteProgram;

typedef struct struct_glDeleteRenderbuffers {
	typ_common_hd hd;
	
	GLsizei n;
	GLuint renderbuffers[MAX_GL_BUFFER_SIZE/sizeof(GLuint)];
}struct_glDeleteRenderbuffers;

typedef struct struct_glDeleteTextures {
	typ_common_hd hd;
	
	GLsizei n;
	GLuint textures[MAX_GL_BUFFER_SIZE/sizeof(GLuint)];
}struct_glDeleteTextures;

typedef struct struct_glDepthFunc {
	typ_common_hd hd;
	
	GLenum func;
}struct_glDepthFunc;

typedef struct struct_glDepthMask {
	typ_common_hd hd;
	
	GLboolean flag;
}struct_glDepthMask;

typedef struct struct_glDepthRangef {
	typ_common_hd hd;
	
	GLclampf zNear;
	GLclampf zFar;
}struct_glDepthRangef;

typedef struct struct_glDetachShader {
	typ_common_hd hd;
	
	GLuint program;
	GLuint shader;
}struct_glDetachShader;

typedef struct struct_glDisable {
	typ_common_hd hd;
	
	GLenum cap;
}struct_glDisable;

typedef struct struct_glDrawElements {
	typ_common_hd hd;
	
	GLenum mode; 
	GLsizei count;
	GLenum type;
	GLint indices_buf_offset;
}struct_glDrawElements;

typedef struct struct_glFinish {
	typ_common_hd hd;
}struct_glFinish;

typedef struct struct_glFlush {
	typ_common_hd hd;
}struct_glFlush;

typedef struct struct_glFramebufferRenderbuffer {
	typ_common_hd hd;
	
	GLenum target;
	GLenum attachment;
	GLenum renderbuffertarget;
	GLuint renderbuffer;
}struct_glFramebufferRenderbuffer;

typedef struct struct_glFramebufferTexture2D {
	typ_common_hd hd;
	
	GLenum target;
	GLenum attachment;
	GLenum textarget;
	GLuint texture;
	GLint level;
}struct_glFramebufferTexture2D;

typedef struct struct_glFrontFace {
	typ_common_hd hd;
	
	GLenum mode;
}struct_glFrontFace;

typedef struct struct_glGenBuffers {
	typ_common_hd hd;
	
	GLsizei n; 
	GLuint buffers[MAX_GL_BUFFER_SIZE/sizeof(GLuint)];
}struct_glGenBuffers;

typedef struct struct_glGenerateMipmap {
	typ_common_hd hd;
	
	GLenum target;
}struct_glGenerateMipmap;

typedef struct struct_glGenFramebuffers {
	typ_common_hd hd;
	
	GLsizei n;
	GLuint framebuffers[MAX_GL_BUFFER_SIZE/sizeof(GLuint)];
}struct_glGenFramebuffers;

typedef struct struct_glGenRenderbuffers {
	typ_common_hd hd;
	
	GLsizei n;
	GLuint renderbuffers[MAX_GL_BUFFER_SIZE/sizeof(GLuint)];
}struct_glGenRenderbuffers;

typedef struct struct_glGenTextures {
	typ_common_hd hd;
	
	GLsizei n;
	GLuint textures[MAX_GL_BUFFER_SIZE/sizeof(GLuint)];
}struct_glGenTextures;

typedef struct struct_glGetActiveAttrib {
	typ_common_hd hd;
	
	GLuint program;
	GLuint index;
	GLsizei bufsize;
	GLsizei length;
	GLint size;
	GLenum type;
	GLuint name_buf_offset;
}struct_glGetActiveAttrib;

typedef struct struct_glGetActiveUniform {
	typ_common_hd hd;
	
	GLuint program;
	GLuint index;
	GLsizei bufsize;
	GLsizei length;
	GLint size;
	GLenum type;
	GLchar name_buf_offset;
}struct_glGetActiveUniform;

typedef struct struct_glGetAttachedShaders {
	typ_common_hd hd;
	
	GLuint program;
	GLsizei maxcount;
	GLsizei count;
	GLuint shaders_buf_offset;
}struct_glGetAttachedShaders;

typedef struct struct_glGetAttribLocation {
	typ_common_hd hd;

	int retVal;

	GLuint program;
	GLchar name_buf_offset;
}struct_glGetAttribLocation;

typedef struct struct_glGetBooleanv {
	typ_common_hd hd;
	
	GLenum pname;
	GLboolean params[16];
}struct_glGetBooleanv;

typedef struct struct_glGetBufferParameteriv {
	typ_common_hd hd;
	
	GLenum target;
	GLenum pname;
	GLint params;
}struct_glGetBufferParameteriv;

typedef struct struct_glGetError {
	typ_common_hd hd;
	GLenum retVal;
}struct_glGetError;

typedef struct struct_glGetFloatv {
	typ_common_hd hd;
	
	GLenum pname;
	GLfloat params[16];
}struct_glGetFloatv;

typedef struct struct_glGetFramebufferAttachmentParameteriv {
	typ_common_hd hd;
	
	GLenum target;
	GLenum attachment;
	GLenum pname;
	GLint params;
}struct_glGetFramebufferAttachmentParameteriv;

typedef struct struct_glGetIntegerv {
	typ_common_hd hd;
	
	GLenum pname;
	GLint params[16];
}struct_glGetIntegerv;

typedef struct struct_glGetRenderbufferParameteriv {
	typ_common_hd hd;
	
	GLenum target;
	GLenum pname;
	GLint params;
}struct_glGetRenderbufferParameteriv;

typedef struct struct_glGetShaderPrecisionFormat {
	typ_common_hd hd;
	
	GLenum shadertype;
	GLenum precisiontype;
	GLint range[2];
	GLint precision;
}struct_glGetShaderPrecisionFormat;

typedef struct struct_glGetShaderSource {
	typ_common_hd hd;
	
	GLuint shader;
	GLsizei bufsize;
	GLsizei length;
	GLint source_buf_offset;
}struct_glGetShaderSource;

typedef struct struct_glGetString {
	typ_common_hd hd;
	
	GLint retVal_buf_offset;
	
	GLenum name;
}struct_glGetString;

typedef struct struct_glGetTexParameterfv {
	typ_common_hd hd;

	GLenum target;
	GLenum pname;
	GLfloat params;
}struct_glGetTexParameterfv;

typedef struct struct_glGetTexParameteriv {
	typ_common_hd hd;
	
	GLenum target;
	GLenum pname;
	GLint params;
}struct_glGetTexParameteriv;

typedef struct struct_glGetUniformfv {
	typ_common_hd hd;
	
	GLuint program;
	GLint location;
	GLuint count;
	GLfloat params[16];	// from a single value to a 4x4 matrix
}struct_glGetUniformfv;

typedef struct struct_glGetUniformiv {
	typ_common_hd hd;
	
	GLuint program;
	GLint location;
	GLuint count;
	GLint params[16];
}struct_glGetUniformiv;

typedef struct struct_glGetVertexAttribfv {
	typ_common_hd hd;
	
	GLuint index;
	GLenum pname;
	GLfloat params[4];
}struct_glGetVertexAttribfv;

typedef struct struct_glGetVertexAttribiv {
	typ_common_hd hd;
	
	GLuint index;
	GLenum pname;
	GLint params[4];
}struct_glGetVertexAttribiv;

typedef struct struct_glGetVertexAttribPointerv {
	typ_common_hd hd;
	
	GLuint index;
	GLenum pname;
	GLvoid* pointer;
}struct_glGetVertexAttribPointerv;

typedef struct struct_glHint {
	typ_common_hd hd;
	
	GLenum target;
	GLenum mode;
}struct_glHint;

typedef struct struct_glIsBuffer {
	typ_common_hd hd;
	
	GLboolean retVal;
	
	GLuint buffer;
}struct_glIsBuffer;

typedef struct struct_glIsEnabled {
	typ_common_hd hd;
	
	GLboolean retVal;
	
	GLenum cap;
}struct_glIsEnabled;

typedef struct struct_glIsFramebuffer {
	typ_common_hd hd;
	
	GLboolean retVal;

	GLuint framebuffer;
}struct_glIsFramebuffer;

typedef struct struct_glIsProgram {
	typ_common_hd hd;
	
	GLboolean retVal;

	GLuint program;
}struct_glIsProgram;

typedef struct struct_glIsRenderbuffer {
	typ_common_hd hd;
	
	GLboolean retVal;

	GLuint renderbuffer;
}struct_glIsRenderbuffer;

typedef struct struct_glIsShader {
	typ_common_hd hd;

	GLboolean retVal;

	GLuint shader;
}struct_glIsShader;

typedef struct struct_glIsTexture {
	typ_common_hd hd;
	
	GLboolean retVal;

	GLuint texture;
}struct_glIsTexture;

typedef struct struct_glLineWidth {
	typ_common_hd hd;
	
	GLfloat width;
}struct_glLineWidth;

typedef struct struct_glPixelStorei {
	typ_common_hd hd;
	
	GLenum pname;
	GLint param;
}struct_glPixelStorei;

typedef struct struct_glPolygonOffset {
	typ_common_hd hd;
	
	GLfloat factor;
	GLfloat units;
}struct_glPolygonOffset;

typedef struct struct_glReadPixels {
	typ_common_hd hd;
	
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLenum type;
	GLint pixels_buf_offset;
}struct_glReadPixels;

typedef struct struct_glReleaseShaderCompiler {
	typ_common_hd hd;
}struct_glReleaseShaderCompiler;

typedef struct struct_glRenderbufferStorage {
	typ_common_hd hd;
	
	GLenum target;
	GLenum internalformat;
	GLsizei width;
	GLsizei height;
}struct_glRenderbufferStorage;

typedef struct struct_glSampleCoverage {
	typ_common_hd hd;
	
	GLclampf value;
	GLboolean invert;
}struct_glSampleCoverage;

typedef struct struct_glScissor {
	typ_common_hd hd;
	
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
}struct_glScissor;

typedef struct struct_glShaderBinary {
	typ_common_hd hd;
	
	GLsizei n;
	GLint shaders_buf_offset;
	GLenum binaryformat;
	GLint binary_buf_offset;
	GLsizei length;
}struct_glShaderBinary;

typedef struct struct_glStencilFunc {
	typ_common_hd hd;
	
	GLenum func;
	GLint ref;
	GLuint mask;
}struct_glStencilFunc;

typedef struct struct_glStencilFuncSeparate {
	typ_common_hd hd;
	
	GLenum face;
	GLenum func;
	GLint ref;
	GLuint mask;
}struct_glStencilFuncSeparate;

typedef struct struct_glStencilMask {
	typ_common_hd hd;
	
	GLuint mask;
}struct_glStencilMask;

typedef struct struct_glStencilMaskSeparate {
	typ_common_hd hd;
	
	GLenum face;
	GLuint mask;
}struct_glStencilMaskSeparate;

typedef struct struct_glStencilOp {
	typ_common_hd hd;
	
	GLenum fail;
	GLenum zfail;
	GLenum zpass;
}struct_glStencilOp;

typedef struct struct_glStencilOpSeparate {
	typ_common_hd hd;
	
	GLenum face;
	GLenum fail;
	GLenum zfail;
	GLenum zpass;
}struct_glStencilOpSeparate;

typedef struct struct_glTexImage2D {
	typ_common_hd hd;
	
	GLenum target;
	GLint level;
	GLint internalformat;
	GLsizei width;
	GLsizei height;
	GLint border;
	GLenum format;
	GLenum type;
	GLint pixels_buf_offset;
}struct_glTexImage2D;

typedef struct struct_glTexParameterf {
	typ_common_hd hd;
	
	GLenum target;
	GLenum pname;
	GLfloat param;
}struct_glTexParameterf;

typedef struct struct_glTexParameterfv {
	typ_common_hd hd;
	
	GLenum target;
	GLenum pname;
	GLfloat params[4];
}struct_glTexParameterfv;

typedef struct struct_glTexParameteri {
	typ_common_hd hd;
	
	GLenum target;
	GLenum pname;
	GLint param;
}struct_glTexParameteri;

typedef struct struct_glTexParameteriv {
	typ_common_hd hd;
	
	GLenum target;
	GLenum pname;
	GLint params[4];
}struct_glTexParameteriv;

typedef struct struct_glTexSubImage2D {
	typ_common_hd hd;
	
	GLenum target;
	GLint level;
	GLint xoffset;
	GLint yoffset;
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLenum type;
	GLint pixels_buf_offset;
}struct_glTexSubImage2D;

typedef struct struct_glUniform1f {
	typ_common_hd hd;
	
	GLint location;
	GLfloat x;
}struct_glUniform1f;

typedef struct struct_glUniform1fv {
	typ_common_hd hd;

	GLint location;
	GLsizei count;	// array count
	GLfloat v[MAX_VERTEX_ARR_SIZE/sizeof(GLfloat)];	// float array for one dimension
}struct_glUniform1fv;

typedef struct struct_glUniform1i {
	typ_common_hd hd;

	GLint location;
	GLint x;
}struct_glUniform1i;

typedef struct struct_glUniform1iv {
	typ_common_hd hd;

	GLint location;
	GLsizei count;
	GLint v[MAX_VERTEX_ARR_SIZE/sizeof(GLint)];	// int array for one dimension
}struct_glUniform1iv;

typedef struct struct_glUniform2f {
	typ_common_hd hd;
	
	GLint location;
	GLfloat x;
	GLfloat y;
}struct_glUniform2f;

typedef struct struct_glUniform2fv {
	typ_common_hd hd;
	
	GLint location;
	GLsizei count;
	GLfloat v[MAX_VERTEX_ARR_SIZE/sizeof(GLfloat)];	// float array for two dimension
}struct_glUniform2fv;

typedef struct struct_glUniform2i {
	typ_common_hd hd;
	
	GLint location;
	GLint x;
	GLint y;
}struct_glUniform2i;

typedef struct struct_glUniform2iv {
	typ_common_hd hd;
	
	GLint location;
	GLsizei count;
	GLint v[MAX_VERTEX_ARR_SIZE/sizeof(GLint)];	// int array for two dimension
}struct_glUniform2iv;

typedef struct struct_glUniform3f {
	typ_common_hd hd;
	
	GLint location;
	GLfloat x;
	GLfloat y;
	GLfloat z;
}struct_glUniform3f;

typedef struct struct_glUniform3fv {
	typ_common_hd hd;
	
	GLint location;
	GLsizei count;
	GLfloat v[MAX_VERTEX_ARR_SIZE/sizeof(GLfloat)];	// float array for three dimension
}struct_glUniform3fv;

typedef struct struct_glUniform3i {
	typ_common_hd hd;
	
	GLint location;
	GLint x;
	GLint y;
	GLint z;
}struct_glUniform3i;

typedef struct struct_glUniform3iv {
	typ_common_hd hd;
	
	GLint location;
	GLsizei count;
	GLint v[MAX_VERTEX_ARR_SIZE/sizeof(GLint)];	// int array for three dimension
}struct_glUniform3iv;

typedef struct struct_glUniform4f {
	typ_common_hd hd;
	
	GLint location;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
}struct_glUniform4f;

typedef struct struct_glUniform4fv {
	typ_common_hd hd;
	
	GLint location;
	GLsizei count;
	GLfloat v[MAX_VERTEX_ARR_SIZE/sizeof(GLfloat)];
}struct_glUniform4fv;

typedef struct struct_glUniform4i {
	typ_common_hd hd;
	
	GLint location;
	GLint x;
	GLint y;
	GLint z;
	GLint w;
}struct_glUniform4i;

typedef struct struct_glUniform4iv {
	typ_common_hd hd;
	
	GLint location;
	GLsizei count;
	GLint v[MAX_VERTEX_ARR_SIZE/sizeof(GLint)];
}struct_glUniform4iv;

typedef struct struct_glUniformMatrix2fv {
	typ_common_hd hd;
	
	GLint location;
	GLsizei count;
	GLboolean transpose;
	GLfloat value[4];		// 2x2 matrix
}struct_glUniformMatrix2fv;

typedef struct struct_glUniformMatrix3fv {
	typ_common_hd hd;
	
	GLint location;
	GLsizei count;
	GLboolean transpose;
	GLfloat value[9];		// 3x3 matrix
}struct_glUniformMatrix3fv;

typedef struct struct_glValidateProgram {
	typ_common_hd hd;
	
	GLuint program;
}struct_glValidateProgram;

typedef struct struct_glVertexAttrib1f {
	typ_common_hd hd;
	
	GLuint indx; 
	GLfloat x;
}struct_glVertexAttrib1f;

typedef struct struct_glVertexAttrib1fv {
	typ_common_hd hd;
	
	GLuint indx;
	// Array to be used for the vertex attribute
	// But, it only modifies the first one component of the vertex array.
	// So, values is one vertex.
	GLfloat values[MAX_VERTEX_ARR_SIZE/sizeof(GLfloat)];
}struct_glVertexAttrib1fv;

typedef struct struct_glVertexAttrib2f {
	typ_common_hd hd;
	
	GLuint indx;
	GLfloat x;
	GLfloat y;
}struct_glVertexAttrib2f;

typedef struct struct_glVertexAttrib2fv {
	typ_common_hd hd;
	
	GLuint indx;
	// Array to be used for the vertex attribute
	// But, it only modifies the first two component of the vertex array.
	// So, values is two vertex.
	GLfloat values[MAX_VERTEX_ARR_SIZE/sizeof(GLfloat)];
}struct_glVertexAttrib2fv;

typedef struct struct_glVertexAttrib3f {
	typ_common_hd hd;
	
	GLuint indx;
	GLfloat x;
	GLfloat y;
	GLfloat z;
}struct_glVertexAttrib3f;

typedef struct struct_glVertexAttrib3fv {
	typ_common_hd hd;
	
	GLuint indx;
	// Array to be used for the vertex attribute
	// But, it only modifies the first three component of the vertex array.
	// So, values is three vertex.
	GLfloat values[MAX_VERTEX_ARR_SIZE/sizeof(GLfloat)];
}struct_glVertexAttrib3fv;

typedef struct struct_glVertexAttrib4f {
	typ_common_hd hd;
	
	GLuint indx;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
}struct_glVertexAttrib4f;

typedef struct struct_glVertexAttrib4fv {
	typ_common_hd hd;
	
	GLuint indx;
	// Array to be used for the vertex attribute
	// But, it only modifies the first four component of the vertex array.
	// So, values is four vertex.
	GLfloat values[MAX_VERTEX_ARR_SIZE/sizeof(GLfloat)];
}struct_glVertexAttrib4fv;

