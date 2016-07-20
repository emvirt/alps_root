#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include <float.h>
#include <limits.h>

#include "wrapStub_struct.h"
#include "functions.h"

extern int dev;
int ret;

// save gl informations for normal world
GLchar infolog[PARAM_SIZE(glGetShaderInfoLog)];
GLchar proginfolog[PARAM_SIZE(glGetProgramInfoLog)];

/*
 * Get the number of components in a pixel format.
 *
 * \param format pixel format.
 *
 * \return the number of components in the given format, or -1 if a bad format.
 */
GLint component_count_in_format(GLenum format)
{
	// removed cases are not supported by GLES 2.0
	switch (format) {
//	case GL_COLOR_INDEX:
	case GL_STENCIL_INDEX:
	case GL_DEPTH_COMPONENT:
//	case GL_RED:
//	case GL_RED_INTEGER_EXT:
//	case GL_GREEN:
//	case GL_GREEN_INTEGER_EXT:
//	case GL_BLUE:
//	case GL_BLUE_INTEGER_EXT:
	case GL_ALPHA:
//	case GL_ALPHA_INTEGER_EXT:
	case GL_LUMINANCE:
//	case GL_LUMINANCE_INTEGER_EXT:
//	case GL_INTENSITY:
		return 1;

	case GL_LUMINANCE_ALPHA:
//	case GL_LUMINANCE_ALPHA_INTEGER_EXT:
//	case GL_RG:
//	case GL_YCBCR_MESA:
//	case GL_DEPTH_STENCIL_EXT:
//	case GL_DUDV_ATI:
//	case GL_DU8DV8_ATI:
//	case GL_RG_INTEGER:
		return 2;

	case GL_RGB:
//	case GL_BGR:
//	case GL_RGB_INTEGER_EXT:
//	case GL_BGR_INTEGER_EXT:
		return 3;

	case GL_RGBA:
//	case GL_BGRA:
//	case GL_ABGR_EXT:
//	case GL_RGBA_INTEGER_EXT:
//	case GL_BGRA_INTEGER_EXT:
		return 4;

	default:
		return -1;
	}
}

GLint get_bytes_per_pixel (GLenum format, GLenum type)
{
	GLint comps = component_count_in_format(format);

	// removed cases are not supported by GLES 2.0
	switch (type) {
//	case GL_BITMAP:
//		return 0;  /* special case */
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		return comps * sizeof(GLubyte);
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
		return comps * sizeof(GLshort);
	case GL_INT:
	case GL_UNSIGNED_INT:
		return comps * sizeof(GLint);
	case GL_FLOAT:
		return comps * sizeof(GLfloat);
//	case GL_HALF_FLOAT_ARB:
//		return comps * sizeof(GLhalfARB);
//	case GL_UNSIGNED_BYTE_3_3_2:
//	case GL_UNSIGNED_BYTE_2_3_3_REV:
//		if (format == GL_RGB || format == GL_BGR ||
//			format == GL_RGB_INTEGER_EXT || format == GL_BGR_INTEGER_EXT)
//			return sizeof(GLubyte);
//		else
//			return -1;  /* error */
	case GL_UNSIGNED_SHORT_5_6_5:
//	case GL_UNSIGNED_SHORT_5_6_5_REV:
		if (format == GL_RGB /*|| format == GL_BGR ||
			format == GL_RGB_INTEGER_EXT || format == GL_BGR_INTEGER_EXT*/)
			return sizeof(GLushort);
		else
			return -1;  /* error */
	case GL_UNSIGNED_SHORT_4_4_4_4:
//	case GL_UNSIGNED_SHORT_4_4_4_4_REV:
	case GL_UNSIGNED_SHORT_5_5_5_1:
//	case GL_UNSIGNED_SHORT_1_5_5_5_REV:
		if (format == GL_RGBA /*|| format == GL_BGRA || format == GL_ABGR_EXT ||
			format == GL_RGBA_INTEGER_EXT || format == GL_BGRA_INTEGER_EXT*/)
			return sizeof(GLushort);
		else
			return -1;
//	case GL_UNSIGNED_INT_8_8_8_8:
//	case GL_UNSIGNED_INT_8_8_8_8_REV:
//	case GL_UNSIGNED_INT_10_10_10_2:
//	case GL_UNSIGNED_INT_2_10_10_10_REV:
//		if (format == GL_RGBA || format == GL_BGRA || format == GL_ABGR_EXT ||
//			format == GL_RGBA_INTEGER_EXT || format == GL_BGRA_INTEGER_EXT ||
//			format == GL_RGB)
//			return sizeof(GLuint);
//		else
//			return -1;
//	case GL_UNSIGNED_SHORT_8_8_MESA:
//	case GL_UNSIGNED_SHORT_8_8_REV_MESA:
//		if (format == GL_YCBCR_MESA)
//			return sizeof(GLushort);
//		else
//			return -1;
//	case GL_UNSIGNED_INT_24_8_EXT:
//		if (format == GL_DEPTH_STENCIL_EXT)
//			return sizeof(GLuint);
//		else
//			return -1;
//	case GL_UNSIGNED_INT_5_9_9_9_REV:
//		if (format == GL_RGB)
//			return sizeof(GLuint);
//		else
//			return -1;
//	case GL_UNSIGNED_INT_10F_11F_11F_REV:
//		if (format == GL_RGB)
//			return sizeof(GLuint);
//		else
//			return -1;
//	case GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
//		if (format == GL_DEPTH_STENCIL)
//			return 8;
//		else
//			return -1;
	default:
			return -1;
	}
}

GLint get_bytes_per_image(GLsizei width, GLsizei height, 
							GLenum format, GLenum type)
{
	GLint bytes_per_pixel = get_bytes_per_pixel(format, type);

//	if(type == GL_BITMAP)
//		return ((width * height) / 8);
//	else
		return ((width * height) * bytes_per_pixel);
}

GLuint identify_pname_type (GLenum pname)
{
	GLuint count = 0;
	
	// removed cases are not supported by GLES 2.0
	switch(pname) {
		case GL_DEPTH_RANGE:
//		case GL_LINE_WIDTH_RANGE:
//		case GL_MAP1_GRID_DOMAIN:
//		case GL_MAP2_GRID_SEGMENTS:
		case GL_MAX_VIEWPORT_DIMS:
//		case GL_POINT_SIZE_RANGE:
//		case GL_POLYGON_MODE:
			count = 2;
			break;
//		case GL_CURRENT_NORMAL:
//			count = 3;
//			break;
//		case GL_ACCUM_CLEAR_VALUE:
		case GL_COLOR_CLEAR_VALUE:
		case GL_COLOR_WRITEMASK:
//		case GL_CURRENT_COLOR:
//		case GL_CURRENT_RASTER_COLOR:
//		case GL_CURRENT_RASTER_POSITION:
//		case GL_CURRENT_RASTER_TEXTURE_COORDS:
//		case GL_CURRENT_TEXTURE_COORDS:
//		case GL_FOG_COLOR:
//		case GL_LIGHT_MODEL_AMBIENT:
//		case GL_MAP2_GRID_DOMAIN:
		case GL_SCISSOR_BOX:
//		case GL_TEXTURE_ENV_COLOR:
		case GL_VIEWPORT:
			count = 4;
			break;
//		case GL_MODELVIEW_MATRIX:
//		case GL_PROJECTION_MATRIX:
//		case GL_TEXTURE_MATRIX:
//			count = 16;
//			break;
		default:
			count = 1;
			break;
	}
	
	return count;
}

int read_without_overflow(int fp, void *buf, int count)
{
	return read(fp, buf, count);
}


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

int call_glEnable(int* pDatabuf)
{
	int ret;
	str_glEnable *function;
	typ_common_hd return_buf;

	function = (str_glEnable *)pDatabuf;

	glEnable(function->cap);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glClear(int* pDatabuf)
{
	str_glClear *function;
	function = (str_glClear *)pDatabuf;
	glClear(function->mask);

	return 0;
}

int call_glClearColor(int* pDatabuf)
{
	str_glClearColor *function;
	function = (str_glClearColor *)pDatabuf;
	glClearColor(function->red, function->green, function->blue, function->alpha);

	return 0;
}


int call_glDisableVertexAttribArray(int *pDatabuf)
{
	str_glDisableVertexAttribArray *function;
	function = (str_glDisableVertexAttribArray *)pDatabuf;
	glDisableVertexAttribArray(function->index);

	return 0;
}

int call_glUniformMatrix4fv(int *pDatabuf)
{
	str_glUniformMatrix4fv *function;
	function = (str_glUniformMatrix4fv *)pDatabuf;

	glUniformMatrix4fv(function->location, function->count, function->transpose, function->value);

	return 0;
}

int call_glEnableVertexAttribArray(int *pDatabuf){
	str_glEnableVertexAttribArray *function;
	function = (str_glEnableVertexAttribArray *)pDatabuf;
	glEnableVertexAttribArray(function->index);

	return 0;
}

int call_glDrawArrays(int *pDatabuf)
{
	str_glDrawArrays *function;

	function = (str_glDrawArrays *)pDatabuf;

	glDrawArrays(function->mode, function->first, function->count);

	return 0;
}

int call_glVertexAttribPointer(int *pDatabuf)
{
	str_glVertexAttribPointer *function;
	GLvoid *vertex_array = NULL;
	
	function = (str_glVertexAttribPointer *)pDatabuf;

	vertex_array = malloc(PARAM_SIZE(glVertexAttribPointer));
	memset(vertex_array, 0, PARAM_SIZE(glVertexAttribPointer));

	memcpy(vertex_array, function->ptr, PARAM_SIZE(glVertexAttribPointer));

	glVertexAttribPointer(function->indx, function->size, function->type, function->normalized, function->stride, vertex_array);

	return 0;
}

int call_glViewport(int *pDatabuf)
{
	str_glViewport *function;
	function = (str_glViewport *)pDatabuf;

	// For normal window
/*	if(function->width > 1920)
		function->width = function->width /2;
	if(function->height > 360)
		function->height = function->height / 2;
	if(function->x < 0)
		function->x = 0;
	if(function->y < 720)
		function->y = 720;
*/
	glViewport(function->x, function->y, function->width, function->height);

	return 0;
}

int call_glCreateShader(int *pDatabuf)
{
	str_glCreateShader *function;
	function = (str_glCreateShader *)pDatabuf;

	function->retVal = glCreateShader(function->shaderType);

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}
	return 0;
}

int call_glShaderSource(int *pDatabuf)
{
	int ret = 0, i;
	void *buf = NULL;
	str_glShaderSource *function;
	typ_common_hd return_buf;

	const GLchar *strings[128] = {0, };
	GLchar *recv_string = NULL;
	GLint *length = NULL;
	unsigned int total_length = 0;

	function = (str_glShaderSource *)pDatabuf;

	total_length = COMM_HEAD_SIZE + function->hd.ldata_length;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	// Copy common header
	memcpy(buf, function, COMM_HEAD_SIZE);
	
	// Read large data buffer
	// Because read size is ldata_length + MAX_BUF_SIZE, 
	// the read size is always bigger than MAX_BUF_SIZE.
	// Read function in kernel copies the large data with "read size - MAX_BUF_SIZE"
	ret = read_without_overflow(dev, (buf + COMM_HEAD_SIZE), (function->hd.ldata_length + MAX_BUF_SIZE));

	function = (str_glShaderSource *)buf;

	recv_string = (GLchar*)(buf + function->str_buf_offset);

	if(function->len_buf_offset > 0) {
		length = (GLint*)(buf + function->len_buf_offset);
		for(i=0; i<function->count; i++) {
			strings[i] = recv_string;
			recv_string = recv_string + length[i] + 1;
		}
	}
	else {
		length = NULL;
		for(i=0; i<function->count; i++) {
			strings[i] = recv_string;
			recv_string = recv_string + strlen(strings[i]) + 1;
		}
	}
	
	glShaderSource(function->shader, function->count, strings, length);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	free(buf);

	return 0;
}

int call_glCompileShader(int *pDatabuf)
{
	str_glCompileShader *function;
	function = (str_glCompileShader *)pDatabuf;
	glCompileShader(function->shader);

	return 0;
}

int call_glGetShaderiv(int *pDatabuf)
{
	str_glGetShaderiv *function;
	GLint params = 0;
	
	function = (str_glGetShaderiv *)pDatabuf;

	glGetShaderiv(function->shader, function->pname, &params);

	function->re_params = params;
	
	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetShaderInfoLog(int *pDatabuf)
{
	str_glGetShaderInfoLog *function;
	GLsizei length = 0;
	GLsizei max_size = PARAM_SIZE(glGetShaderInfoLog);
	
	function = (str_glGetShaderInfoLog *)pDatabuf;
	glGetShaderInfoLog(function->shader, function->maxLength, &length, infolog);

	if(length > max_size)
		length = max_size;
	
	memcpy(function->re_infoLog, infolog, length);
	function->re_length = length;

	if(function->hd.retFlag == RT_REQUEST) {
			function->hd.retFlag = RT_RESPONSE;
			ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glCreateProgram(int *pDatabuf)
{
	str_glCreateProgram *function;
	function = (str_glCreateProgram *)pDatabuf;
	function->retVal = glCreateProgram();

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}
	return 0;
}

int call_glAttachShader(int *pDatabuf)
{
	str_glAttachShader *function;
	function = (str_glAttachShader *)pDatabuf;

	glAttachShader(function->program, function->shader);

	return 0;
}

int call_glBindAttribLocation(int *pDatabuf)
{
	str_glBindAttribLocation *function;
	GLchar *name;
	
	function = (str_glBindAttribLocation *)pDatabuf;
	if(function->name[0] == 0)
		name = NULL;
	else
		name = function->name;
	
	glBindAttribLocation(function->program, function->index, name);

	return 0;
}

int call_glLinkProgram(int *pDatabuf)
{
	str_glLinkProgram *function;
	function = (str_glLinkProgram *)pDatabuf;
	glLinkProgram(function->program);

	return 0;
}

int call_glGetProgramiv(int *pDatabuf)
{
	str_glGetProgramiv *function;
	GLint params = 0;
	
	function = (str_glGetProgramiv *)pDatabuf;
	glGetProgramiv(function->program, function->pname, &params);

	function->re_params = params;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetProgramInfoLog(int *pDatabuf)
{
	str_glGetProgramInfoLog *function;
	GLsizei length = 0;
	GLsizei max_size = PARAM_SIZE(glGetProgramInfoLog);
	
	function = (str_glGetProgramInfoLog *)pDatabuf;
	glGetProgramInfoLog(function->program, function->maxLength, &length, proginfolog);

	if(length > max_size)
		length = max_size;
	
	memcpy(function->re_infoLog, proginfolog, length);
	function->re_length = length;

	if(function->hd.retFlag == RT_REQUEST) {
			function->hd.retFlag = RT_RESPONSE;
			ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetUniformLocation(int *pDatabuf)
{
	int ret;
	str_glGetUniformLocation *function;
	GLchar *name;
	void *buf;
	unsigned int ldata_length = 0;
	unsigned int total_length = 0;
	
	function = (str_glGetUniformLocation *)pDatabuf;

	ldata_length = function->hd.ldata_length;
	if(ldata_length > 0) {
		total_length = ldata_length + COMM_HEAD_SIZE;

		buf = (void *)malloc(total_length);
		memset(buf, 0, total_length);
		memcpy(buf, function, COMM_HEAD_SIZE);

		ret = read_without_overflow(dev, (buf+COMM_HEAD_SIZE), (ldata_length + MAX_BUF_SIZE));
	}
	else {
		total_length = sizeof(str_glGetUniformLocation) + function->size + 1;
		buf = (void*)pDatabuf;
	}

	function = (str_glGetUniformLocation *)buf;
	name = (GLchar*)(buf + function->name_buf_offset);
	
	function->retVal = glGetUniformLocation(function->program, name);

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, (COMM_HEAD_SIZE + sizeof(GLint)));
	}

	if(ldata_length > 0)
		free(buf);

	return 0;
}

int call_glDeleteShader(int *pDatabuf)
{
	str_glDeleteShader *function;
	function = (str_glDeleteShader *)pDatabuf;
	glDeleteShader(function->shader);

	return 0;
}


int call_glUseProgram(int *pDatabuf)
{
	str_glUseProgram *function;
	function = (str_glUseProgram *)pDatabuf;
	glUseProgram(function->program);

	return 0;
}

////////////////////////////
int call_glActiveTexture(int *pDatabuf)
{
	struct_glActiveTexture *function;

	function = (struct_glActiveTexture *)pDatabuf;

	glActiveTexture(function->texture);

	return 0;	
}

int call_glBindBuffer(int *pDatabuf)
{
	struct_glBindBuffer *function;

	function = (struct_glBindBuffer *)pDatabuf;

	glBindBuffer(function->target, function->buffer);

	return 0;
}

int call_glBindFramebuffer(int *pDatabuf)
{
	struct_glBindFramebuffer *function;

	function = (struct_glBindFramebuffer *)pDatabuf;

	glBindFramebuffer(function->target, function->framebuffer);

	return 0;
}

int call_glBindRenderbuffer(int *pDatabuf)
{
	struct_glBindRenderbuffer *function;

	function = (struct_glBindRenderbuffer *)pDatabuf;

	glBindRenderbuffer(function->target, function->renderbuffer);

	return 0;
}

int call_glBindTexture(int *pDatabuf)
{
	struct_glBindTexture *function;
	typ_common_hd readhd;

	function = (struct_glBindTexture *)pDatabuf;

	glBindTexture(function->target, function->texture);

	if(function->hd.retFlag == RT_REQUEST){
		readhd.funcID = function->hd.funcID;
		readhd.retFlag = RT_RESPONSE;
		readhd.ldata_length = 0;
		ret = write(dev, &readhd, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glBlendColor(int *pDatabuf)
{
	struct_glBlendColor *function;

	function = (struct_glBlendColor *)pDatabuf;

	glBlendColor(function->red, function->green, function->blue, function->alpha);

	return 0;
}

int call_glBlendEquation(int *pDatabuf)
{
	struct_glBlendEquation *function;

	function = (struct_glBlendEquation *)pDatabuf;

	glBlendEquation(function->mode);

	return 0;
}

int call_glBlendEquationSeparate(int *pDatabuf)
{
	struct_glBlendEquationSeparate *function;

	function = (struct_glBlendEquationSeparate *)pDatabuf;

	glBlendEquationSeparate(function->modeRGB, function->modeAlpha);

	return 0;
}

int call_glBlendFunc(int *pDatabuf)
{
	struct_glBlendFunc *function;

	function = (struct_glBlendFunc *)pDatabuf;

	glBlendFunc(function->sfactor, function->dfactor);

	return 0;
}

int call_glBlendFuncSeparate(int *pDatabuf)
{
	struct_glBlendFuncSeparate *function;

	function = (struct_glBlendFuncSeparate *)pDatabuf;

	glBlendFuncSeparate(function->srcRGB, function->dstRGB, function->srcAlpha, function->dstAlpha);

	return 0;
}

int call_glBufferData(int *pDatabuf)
{
	void *buf = NULL;
	struct_glBufferData *function;
	typ_common_hd return_buf;
	const GLvoid *data = NULL;
	int ret = 0;
	unsigned int total_length = 0;
	unsigned int ldata_length = 0;

	total_length = COMM_HEAD_SIZE + MAX_LDATA_SIZE;
	ldata_length = MAX_LDATA_SIZE;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	// Copy common header
	memcpy(buf, pDatabuf, COMM_HEAD_SIZE);

	function = (struct_glBufferData *)buf;
	if(function->hd.ldata_length > 0) {
		// Read large data buffer
		// Because read size is ldata_length + MAX_BUF_SIZE, 
		// the read size is always bigger than MAX_BUF_SIZE.
		// Read function in kernel copies the large data with "read size - MAX_BUF_SIZE"
		ret = read_without_overflow(dev, (buf + COMM_HEAD_SIZE), (ldata_length + MAX_BUF_SIZE));
		data = (GLvoid*)(buf + function->data_buf_offset);
	}
	
	glBufferData(function->target, function->size, data, function->usage);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	free(buf);

	return 0;
}

int call_glBufferSubData(int *pDatabuf)
{
	void *buf = NULL;
	struct_glBufferSubData *function;
	typ_common_hd return_buf;
	const GLvoid *data = NULL;
	int ret = 0;
	unsigned int total_length = 0;
	unsigned int ldata_length = 0;

	total_length = COMM_HEAD_SIZE + MAX_LDATA_SIZE;
	ldata_length = MAX_LDATA_SIZE;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	// Copy common header
	memcpy(buf, pDatabuf, COMM_HEAD_SIZE);

	function = (struct_glBufferSubData *)buf;
	if(function->hd.ldata_length > 0) {
		// Read large data buffer
		// Because read size is ldata_length + MAX_BUF_SIZE, 
		// the read size is always bigger than MAX_BUF_SIZE.
		// Read function in kernel copies the large data with "read size - MAX_BUF_SIZE"
		ret = read_without_overflow(dev, (buf + COMM_HEAD_SIZE), (ldata_length + MAX_BUF_SIZE));
		data = (GLvoid*)(buf + function->data_buf_offset);
	}
	
	glBufferSubData(function->target, function->offset, function->size, data);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	free(buf);

	return 0;
}

int call_glCheckFramebufferStatus(int *pDatabuf)
{
	struct_glCheckFramebufferStatus *function;
	GLenum status;

	function = (struct_glCheckFramebufferStatus *)pDatabuf;

	status = glCheckFramebufferStatus(function->target);

	function->retVal = status;

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glClearDepthf(int *pDatabuf)
{
	struct_glClearDepthf *function;

	function = (struct_glClearDepthf *)pDatabuf;

	glClearDepthf(function->depth);

	return 0;
}

int call_glClearStencil(int *pDatabuf)
{
	struct_glClearStencil *function;

	function = (struct_glClearStencil *)pDatabuf;

	glClearStencil(function->s);

	return 0;
}

int call_glColorMask(int *pDatabuf)
{
	struct_glColorMask *function;

	function = (struct_glColorMask *)pDatabuf;

	glColorMask(function->red, function->green, function->blue, function->alpha);

	return 0;
}

int call_glCompressedTexImage2D(int *pDatabuf)
{
	void *buf = NULL;
	struct_glCompressedTexImage2D *function;
	typ_common_hd return_buf;
	const GLvoid *data = NULL;
	int ret = 0;
	unsigned int total_length = 0;
	unsigned int ldata_length = 0;

	total_length = COMM_HEAD_SIZE + MAX_LDATA_SIZE;
	ldata_length = MAX_LDATA_SIZE;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	// Copy common header
	memcpy(buf, pDatabuf, COMM_HEAD_SIZE);

	function = (struct_glCompressedTexImage2D *)buf;
	if(function->hd.ldata_length > 0) {
		// Read large data buffer
		// Because read size is ldata_length + MAX_BUF_SIZE, 
		// the read size is always bigger than MAX_BUF_SIZE.
		// Read function in kernel copies the large data with "read size - MAX_BUF_SIZE"
		ret = read_without_overflow(dev, (buf + COMM_HEAD_SIZE), (ldata_length + MAX_BUF_SIZE));
		data = (GLvoid*)(buf + function->data_buf_offset);
	}
	
	glCompressedTexImage2D (function->target, 
							function->level, function->internalformat, 
							function->width, function->height, 
							function->border, function->imageSize, data);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	free(buf);

	return 0;
}

int call_glCompressedTexSubImage2D(int *pDatabuf)
{
	void *buf = NULL;
	struct_glCompressedTexSubImage2D *function;
	typ_common_hd return_buf;
	const GLvoid *data = NULL;
	int ret = 0;
	unsigned int total_length = 0;
	unsigned int ldata_length = 0;

	total_length = COMM_HEAD_SIZE + MAX_LDATA_SIZE;
	ldata_length = MAX_LDATA_SIZE;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	// Copy common header
	memcpy(buf, pDatabuf, COMM_HEAD_SIZE);

	function = (struct_glCompressedTexSubImage2D *)buf;
	if(function->hd.ldata_length > 0) {
		// Read large data buffer
		// Because read size is ldata_length + MAX_BUF_SIZE, 
		// the read size is always bigger than MAX_BUF_SIZE.
		// Read function in kernel copies the large data with "read size - MAX_BUF_SIZE"
		ret = read_without_overflow(dev, (buf + COMM_HEAD_SIZE), (ldata_length + MAX_BUF_SIZE));
		data = (GLvoid*)(buf + function->data_buf_offset);
	}
	
	glCompressedTexSubImage2D (function->target, 
							function->level, function->xoffset, function->yoffset,
							function->width, function->height, function->format, 
							function->imageSize, data);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	free(buf);

	return 0;
}

int call_glCopyTexImage2D(int *pDatabuf)
{
	struct_glCopyTexImage2D *function;

	function = (struct_glCopyTexImage2D *)pDatabuf;

	glCopyTexImage2D(function->target, function->level, 
					function->internalformat, function->x, function->y,
					function->width, function->height, function->border);

	return 0;
}

int call_glCopyTexSubImage2D(int *pDatabuf)
{
	struct_glCopyTexSubImage2D *function;

	function = (struct_glCopyTexSubImage2D *)pDatabuf;

	glCopyTexSubImage2D(function->target, function->level, 
					function->xoffset, function->yoffset, function->x, function->y,
					function->width, function->height);

	return 0;
}

int call_glCullFace(int *pDatabuf)
{
	struct_glCullFace *function;

	function = (struct_glCullFace *)pDatabuf;

	glCullFace(function->mode);

	return 0;
}

int call_glDeleteBuffers(int *pDatabuf)
{
	int ret;
	struct_glDeleteBuffers *function;
	typ_common_hd return_buf;
	const GLuint *buffers = NULL;

	function = (struct_glDeleteBuffers *)pDatabuf;
	buffers = function->buffers;

	glDeleteBuffers(function->n, buffers);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glDeleteFramebuffers(int *pDatabuf)
{
	int ret;
	struct_glDeleteFramebuffers *function;
	typ_common_hd return_buf;
	const GLuint *buffers = NULL;

	function = (struct_glDeleteFramebuffers *)pDatabuf;
	buffers = function->framebuffers;

	glDeleteFramebuffers(function->n, buffers);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glDeleteProgram(int *pDatabuf)
{
	int ret;
	struct_glDeleteProgram *function;
	typ_common_hd return_buf;

	function = (struct_glDeleteProgram *)pDatabuf;

	glDeleteProgram(function->program);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glDeleteRenderbuffers(int *pDatabuf)
{
	int ret;
	struct_glDeleteRenderbuffers *function;
	typ_common_hd return_buf;
	const GLuint *buffers = NULL;

	function = (struct_glDeleteRenderbuffers *)pDatabuf;
	buffers = function->renderbuffers;

	glDeleteRenderbuffers(function->n, buffers);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glDeleteTextures(int *pDatabuf)
{
	int ret;
	struct_glDeleteTextures *function;
	typ_common_hd return_buf;
	const GLuint *buffers = NULL;

	function = (struct_glDeleteTextures *)pDatabuf;
	buffers = function->textures;

	glDeleteTextures(function->n, buffers);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glDepthFunc(int *pDatabuf)
{
	struct_glDepthFunc *function;

	function = (struct_glDepthFunc *)pDatabuf;

	glDepthFunc(function->func);

	return 0;
}

int call_glDepthMask(int *pDatabuf)
{
	struct_glDepthMask *function;

	function = (struct_glDepthMask *)pDatabuf;

	glDepthMask(function->flag);

	return 0;
}

int call_glDepthRangef(int *pDatabuf)
{
	struct_glDepthRangef *function;

	function = (struct_glDepthRangef *)pDatabuf;

	glDepthRangef(function->zNear, function->zFar);

	return 0;
}

int call_glDetachShader(int *pDatabuf)
{
	struct_glDetachShader *function;

	function = (struct_glDetachShader *)pDatabuf;

	glDetachShader(function->program, function->shader);

	return 0;
}

int call_glDisable(int *pDatabuf)
{
	int ret;
	struct_glDisable *function;
	typ_common_hd return_buf;

	function = (struct_glDisable *)pDatabuf;

	glDisable(function->cap);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glDrawElements(int *pDatabuf)
{
	int ret = 0, i=0;
	struct_glDrawElements *function;
	typ_common_hd return_buf;

	GLvoid *indices = NULL;
	GLvoid *cur_indices = NULL;

	GLsizei data_length = 0;
	GLsizei last_ldata_length = 0;
	GLsizei ldata_count = 0;

	function = (struct_glDrawElements *)pDatabuf;

	data_length = function->hd.ldata_length;

	if(data_length > 0) {
		ldata_count = data_length / MAX_LDATA_SIZE_1M;
		last_ldata_length = data_length % MAX_LDATA_SIZE_1M;

		indices = (GLvoid*)malloc(data_length);
		memset(indices, 0, data_length);
		for(i=0; i<ldata_count; i++) {
			// request "pixels"
			function->hd.retFlag = RT_RESPONSE;
			ret = write(dev, function, COMM_HEAD_SIZE);

			cur_indices = indices + (MAX_LDATA_SIZE_1M * i);
			ret = read_without_overflow(dev, cur_indices, (MAX_LDATA_SIZE_1M + MAX_BUF_SIZE));
		}

		if(last_ldata_length > 0) {
			// request the last data of "pixels"
			function->hd.retFlag = RT_RESPONSE;
			ret = write(dev, function, COMM_HEAD_SIZE);

			cur_indices = indices + (MAX_LDATA_SIZE_1M * i);
			ret = read_without_overflow(dev, cur_indices, (last_ldata_length + MAX_BUF_SIZE));
		}
	}
	
	glDrawElements (function->mode, function->count, function->type, indices);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	if(indices != NULL)
		free(indices);

	return 0;
}

int call_glFinish(int *pDatabuf)
{
	struct_glFinish *function;

	function = (struct_glFinish *)pDatabuf;

	glFinish();

	return 0;
}

int call_glFlush(int *pDatabuf)
{
	int ret;
	struct_glFlush *function;
	typ_common_hd return_buf;

	function = (struct_glFlush *)pDatabuf;

	glFlush();

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glFramebufferRenderbuffer(int *pDatabuf)
{
	int ret;
	struct_glFramebufferRenderbuffer *function;
	typ_common_hd return_buf;

	function = (struct_glFramebufferRenderbuffer *)pDatabuf;

	glFramebufferRenderbuffer(function->target, function->attachment, 
						function->renderbuffertarget, function->renderbuffer);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glFramebufferTexture2D(int *pDatabuf)
{
	int ret;
	struct_glFramebufferTexture2D *function;
	typ_common_hd return_buf;

	function = (struct_glFramebufferTexture2D *)pDatabuf;

	glFramebufferTexture2D(function->target, function->attachment, 
						function->textarget, function->texture, function->level);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glFrontFace(int *pDatabuf)
{
	struct_glFrontFace *function;

	function = (struct_glFrontFace *)pDatabuf;

	glFrontFace(function->mode);

	return 0;
}

int call_glGenBuffers(int *pDatabuf)
{
	int ret;
	struct_glGenBuffers *function;
	GLuint *bufs = NULL;
	unsigned int byte_len = 0;

	function = (struct_glGenBuffers *)pDatabuf;

	byte_len = sizeof(GLuint) * function->n;
	bufs = (GLuint *)malloc(byte_len);
	
	glGenBuffers(function->n, bufs);

	memcpy(function->buffers, bufs, byte_len);

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGenerateMipmap(int *pDatabuf)
{
	int ret;
	struct_glGenerateMipmap *function;
	typ_common_hd return_buf;

	function = (struct_glGenerateMipmap *)pDatabuf;
	
	glGenerateMipmap(function->target);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glGenFramebuffers(int *pDatabuf)
{
	int ret;
	struct_glGenFramebuffers *function;
	GLuint *bufs = NULL;
	unsigned int byte_len = 0;

	function = (struct_glGenFramebuffers *)pDatabuf;

	byte_len = sizeof(GLuint) * function->n;
	bufs = (GLuint *)malloc(byte_len);
	
	glGenFramebuffers(function->n, bufs);

	memcpy(function->framebuffers, bufs, byte_len);

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGenRenderbuffers(int *pDatabuf)
{
	int ret;
	struct_glGenRenderbuffers *function;
	GLuint *bufs = NULL;
	unsigned int byte_len = 0;

	function = (struct_glGenRenderbuffers *)pDatabuf;

	byte_len = sizeof(GLuint) * function->n;
	bufs = (GLuint *)malloc(byte_len);
	
	glGenRenderbuffers(function->n, bufs);

	memcpy(function->renderbuffers, bufs, byte_len);

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGenTextures(int *pDatabuf)
{
	int ret;
	struct_glGenTextures *function;
	GLuint *bufs = NULL;
	unsigned int byte_len = 0;

	function = (struct_glGenTextures *)pDatabuf;

	byte_len = sizeof(GLuint) * function->n;
	bufs = (GLuint *)malloc(byte_len);
	
	glGenTextures(function->n, bufs);

	memcpy(function->textures, bufs, byte_len);

	if(function->hd.retFlag == RT_REQUEST){
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetActiveAttrib(int *pDatabuf)
{
	int ret;
	struct_glGetActiveAttrib *function;
	struct_glGetActiveAttrib *writebuf;
	void *return_buf = NULL;

	GLsizei length;
	GLint size;
	GLenum type;
	GLchar *name = NULL;
	
	GLsizei struct_length = sizeof(struct_glGetActiveAttrib);
	GLsizei total_length = 0;

	function = (struct_glGetActiveAttrib *)pDatabuf;

	glGetActiveAttrib(function->program, function->index, function->bufsize,
				&length, &size, &type, name);

	// include null terminator
	total_length = struct_length + length + 1;
	
	return_buf = (void*)malloc(total_length);
	memset(return_buf, 0, total_length);
	
	writebuf = (struct_glGetActiveAttrib *)return_buf;

	writebuf->hd.funcID = function->hd.funcID;
	writebuf->hd.retFlag = RT_RESPONSE;
	writebuf->program = function->program;
	writebuf->index = function->index;
	writebuf->bufsize = function->bufsize;
	writebuf->length = length;
	writebuf->size = size;
	writebuf->name_buf_offset = struct_length;

	if(length > 0) {
		writebuf->hd.ldata_length = total_length - COMM_HEAD_SIZE;
		memcpy((return_buf + writebuf->name_buf_offset), name, length + 1);

		// Because write size is total_length + MAX_BUF_SIZE, 
		// the write size is always bigger than MAX_BUF_SIZE.
		// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
		ret = write(dev, return_buf, (total_length + MAX_BUF_SIZE));
	}
	else {
		ret = write(dev, return_buf, total_length);
	}

	free(name);
	free(return_buf);

	return 0;
}

int call_glGetActiveUniform(int *pDatabuf)
{
	int ret;
	struct_glGetActiveUniform *function;
	struct_glGetActiveUniform *writebuf;
	void *return_buf = NULL;

	GLsizei length;
	GLint size;
	GLenum type;
	GLchar *name = NULL;
	
	GLsizei struct_length = sizeof(struct_glGetActiveUniform);
	GLsizei total_length = 0;

	function = (struct_glGetActiveUniform *)pDatabuf;

	glGetActiveUniform(function->program, function->index, function->bufsize,
				&length, &size, &type, name);

	// include null terminator
	total_length = struct_length + length + 1;
	
	return_buf = (void*)malloc(total_length);
	memset(return_buf, 0, total_length);
	
	writebuf = (struct_glGetActiveUniform *)return_buf;

	writebuf->hd.funcID = function->hd.funcID;
	writebuf->hd.retFlag = RT_RESPONSE;
	writebuf->program = function->program;
	writebuf->index = function->index;
	writebuf->bufsize = function->bufsize;
	writebuf->length = length;
	writebuf->size = size;
	writebuf->name_buf_offset = struct_length;

	if(length > 0) {
		writebuf->hd.ldata_length = total_length - COMM_HEAD_SIZE;
		memcpy((return_buf + writebuf->name_buf_offset), name, length + 1);

		// Because write size is total_length + MAX_BUF_SIZE, 
		// the write size is always bigger than MAX_BUF_SIZE.
		// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
		ret = write(dev, return_buf, (total_length + MAX_BUF_SIZE));
	}
	else {
		ret = write(dev, return_buf, total_length);
	}

	free(name);
	free(return_buf);

	return 0;
}

int call_glGetAttachedShaders(int *pDatabuf)
{
	int ret;
	struct_glGetAttachedShaders *function;
	struct_glGetAttachedShaders *writebuf;
	void *return_buf = NULL;

	GLsizei count_byte = 0;
	GLsizei count;
	GLuint *shaders = NULL;
	
	GLsizei struct_length = sizeof(struct_glGetAttachedShaders);
	GLsizei total_length = 0;

	function = (struct_glGetAttachedShaders *)pDatabuf;

	glGetAttachedShaders(function->program, function->maxcount, &count, shaders);

	count_byte = count * sizeof(GLuint);
	total_length = struct_length + count_byte;
	
	return_buf = (void*)malloc(total_length);
	memset(return_buf, 0, total_length);
	
	writebuf = (struct_glGetAttachedShaders *)return_buf;

	writebuf->hd.funcID = function->hd.funcID;
	writebuf->hd.retFlag = RT_RESPONSE;
	writebuf->program = function->program;
	writebuf->maxcount = function->maxcount;
	writebuf->count = count;
	writebuf->shaders_buf_offset = struct_length;

	if(count > 0) {
		writebuf->hd.ldata_length = total_length - COMM_HEAD_SIZE;
		memcpy((return_buf + writebuf->shaders_buf_offset), shaders, count_byte);

		// Because write size is total_length + MAX_BUF_SIZE, 
		// the write size is always bigger than MAX_BUF_SIZE.
		// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
		ret = write(dev, return_buf, (total_length + MAX_BUF_SIZE));
	}
	else {
		ret = write(dev, return_buf, total_length);
	}

	free(shaders);
	free(return_buf);

	return 0;
}

int call_glGetAttribLocation(int *pDatabuf)
{
	int ret;
	int index;
	struct_glGetAttribLocation *function;
	struct_glGetAttribLocation return_buf;
	void *buf = NULL;
	GLchar *name = NULL;
	
	GLsizei struct_length = sizeof(struct_glGetAttribLocation);
	GLsizei name_length = 0;
	GLsizei total_length = 0;

	function = (struct_glGetAttribLocation *)pDatabuf;

	total_length = COMM_HEAD_SIZE + function->hd.ldata_length;
	name_length = total_length - struct_length;	// include null terminator
	
	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	memcpy(buf, pDatabuf, COMM_HEAD_SIZE);

	function = (struct_glGetAttribLocation *)buf;

	if(function->hd.ldata_length > 0) {
		ret = read_without_overflow(dev, (buf + COMM_HEAD_SIZE), (function->hd.ldata_length + MAX_BUF_SIZE));
		name = (GLchar *)malloc(name_length);
		memcpy(name, (buf + function->name_buf_offset), name_length);
	}

	index = glGetAttribLocation(function->program, name);

	if(function->hd.retFlag == RT_REQUEST) {
		return_buf.hd.funcID = function->hd.funcID;
		return_buf.hd.retFlag = RT_RESPONSE;
		return_buf.hd.ldata_length = 0;
		return_buf.retVal = index;
		return_buf.name_buf_offset = 0;

		ret = write(dev, &return_buf, MAX_BUF_SIZE);
	}

	free(name);
	free(buf);

	return 0;
}

int call_glGetBooleanv(int *pDatabuf)
{
	int ret, i;
	struct_glGetBooleanv *function;
	GLboolean params[16] = {0, };
	GLuint value_count = 0;
	
	function = (struct_glGetBooleanv *)pDatabuf;

	glGetBooleanv(function->pname, params);

	value_count = identify_pname_type(function->pname);

	for(i=0; i<value_count; i++) {
		function->params[i] = params[i];
	}

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetBufferParameteriv(int *pDatabuf)
{
	int ret;
	struct_glGetBufferParameteriv *function;
	GLint params;
	
	function = (struct_glGetBufferParameteriv *)pDatabuf;

	glGetBufferParameteriv(function->target, function->pname, &params);

	function->params = params;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetError(int *pDatabuf)
{
	int ret;
	struct_glGetError *function;
	GLenum err_value;
	
	function = (struct_glGetError *)pDatabuf;

	err_value = glGetError();

	function->retVal = err_value;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetFloatv(int *pDatabuf)
{
	int ret, i;
	struct_glGetFloatv *function;
	GLfloat params[16] = {0, };
	GLuint value_count = 0;
	
	function = (struct_glGetFloatv *)pDatabuf;

	glGetFloatv(function->pname, params);

	value_count = identify_pname_type(function->pname);

	for(i=0; i<value_count; i++) {
		function->params[i] = params[i];
	}

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetFramebufferAttachmentParameteriv(int *pDatabuf)
{
	int ret;
	struct_glGetFramebufferAttachmentParameteriv *function;
	GLint params;
	
	function = (struct_glGetFramebufferAttachmentParameteriv *)pDatabuf;

	glGetFramebufferAttachmentParameteriv(function->target, function->attachment, function->pname, &params);

	function->params = params;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetIntegerv(int *pDatabuf)
{
	int ret, i;
	struct_glGetIntegerv *function;
	GLint params[16] = {0, };
	GLuint value_count = 0;
	
	function = (struct_glGetIntegerv *)pDatabuf;

	glGetIntegerv(function->pname, params);

	value_count = identify_pname_type(function->pname);

	for(i=0; i<value_count; i++) {
		function->params[i] = params[i];
	}

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetRenderbufferParameteriv(int *pDatabuf)
{
	int ret;
	struct_glGetRenderbufferParameteriv *function;
	GLint params;
	
	function = (struct_glGetRenderbufferParameteriv *)pDatabuf;

	glGetRenderbufferParameteriv(function->target, function->pname, &params);

	function->params = params;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetShaderPrecisionFormat(int *pDatabuf)
{
	int ret;
	struct_glGetShaderPrecisionFormat *function;
	GLint range[2] = {0, };
	GLint precision;
	
	function = (struct_glGetShaderPrecisionFormat *)pDatabuf;

	glGetShaderPrecisionFormat(function->shadertype, function->precisiontype, range, &precision);

	function->range[0] = range[0];
	function->range[1] = range[1];
	function->precision = precision;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetShaderSource(int *pDatabuf)
{
	int ret;
	struct_glGetShaderSource *function;
	struct_glGetShaderSource *writebuf;
	void *return_buf = NULL;
	GLchar *source = NULL;
	GLsizei source_length = 0;
	
	GLsizei struct_length = sizeof(struct_glGetShaderSource);
	GLsizei total_length = 0;

	function = (struct_glGetShaderSource *)pDatabuf;

	source = (GLchar *)malloc(function->bufsize);
	memset(source, 0, function->bufsize);
	
	glGetShaderSource(function->shader, function->bufsize, &source_length, source);

	// include null terminator
	total_length = struct_length + source_length + 1;
	
	return_buf = (void*)malloc(total_length);
	memset(return_buf, 0, total_length);
	
	writebuf = (struct_glGetShaderSource *)return_buf;

	writebuf->hd.funcID = function->hd.funcID;
	writebuf->hd.retFlag = RT_RESPONSE;
	writebuf->shader = function->shader;
	writebuf->bufsize = function->bufsize;
	writebuf->length = source_length;
	writebuf->source_buf_offset = struct_length;

	if(source_length > 0) {
		writebuf->hd.ldata_length = total_length - COMM_HEAD_SIZE;
		memcpy((return_buf + writebuf->source_buf_offset), source, source_length + 1);

		// Because write size is total_length + MAX_BUF_SIZE, 
		// the write size is always bigger than MAX_BUF_SIZE.
		// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
		ret = write(dev, return_buf, (total_length + MAX_BUF_SIZE));
	}
	else {
		ret = write(dev, return_buf, total_length);
	}

	free(source);
	free(return_buf);

	return 0;
}

int call_glGetString(int *pDatabuf)
{
	int ret;
	struct_glGetString *function;
	struct_glGetString *writebuf;
	void *buf = NULL;
	const GLubyte *ret_string = NULL;
	GLsizei ret_string_length = 0;
	
	GLsizei struct_length = sizeof(struct_glGetString);
	GLsizei total_length = 0;

	function = (struct_glGetString *)pDatabuf;

	ret_string = glGetString(function->name);

	ret_string_length = strlen((char*)ret_string);
	// include null terminator
	total_length = struct_length + ret_string_length + 1;
	
	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);
	
	writebuf = (struct_glGetString *)buf;

	writebuf->hd.funcID = function->hd.funcID;
	writebuf->hd.retFlag = RT_RESPONSE;
	writebuf->name = function->name;
	writebuf->retVal_buf_offset = struct_length;

	if(ret_string_length > 0) {
		writebuf->hd.ldata_length = total_length - COMM_HEAD_SIZE;
		memcpy((buf + writebuf->retVal_buf_offset), ret_string, ret_string_length + 1);

		// Because write size is total_length + MAX_BUF_SIZE, 
		// the write size is always bigger than MAX_BUF_SIZE.
		// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
		ret = write(dev, buf, (total_length + MAX_BUF_SIZE));
	}
	else {
		ret = write(dev, buf, total_length);
	}

	free(buf);

	return 0;
}

int call_glGetTexParameterfv(int *pDatabuf)
{
	int ret;
	struct_glGetTexParameterfv *function;
	GLfloat params;
	
	function = (struct_glGetTexParameterfv *)pDatabuf;

	glGetTexParameterfv(function->target, function->pname, &params);

	function->params = params;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetTexParameteriv(int *pDatabuf)
{
	int ret;
	struct_glGetTexParameteriv *function;
	GLint params;
	
	function = (struct_glGetTexParameteriv *)pDatabuf;

	glGetTexParameteriv(function->target, function->pname, &params);

	function->params = params;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetUniformfv(int *pDatabuf)
{
	int ret, i;
	struct_glGetUniformfv *function;
	
	function = (struct_glGetUniformfv *)pDatabuf;
	for(i=0; i<16; i++) {
		function->params[i] = FLT_MAX;
	}

	glGetUniformfv(function->program, function->location, function->params);

	for(i=0; i<16; i++) {
		if(function->params[i] == FLT_MAX)
			break;
	}

	function->count = i;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetUniformiv(int *pDatabuf)
{
	int ret, i;
	struct_glGetUniformiv *function;
	
	function = (struct_glGetUniformiv *)pDatabuf;
	for(i=0; i<16; i++) {
		function->params[i] = INT_MAX;
	}

	glGetUniformiv(function->program, function->location, function->params);

	for(i=0; i<16; i++) {
		if(function->params[i] == INT_MAX)
			break;
	}

	function->count = i;

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetVertexAttribfv(int *pDatabuf)
{
	int ret;
	struct_glGetVertexAttribfv *function;
	
	function = (struct_glGetVertexAttribfv *)pDatabuf;
	memset(function->params, 0, sizeof(GLfloat) * 4);

	glGetVertexAttribfv(function->index, function->pname, function->params);

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetVertexAttribiv(int *pDatabuf)
{
	int ret;
	struct_glGetVertexAttribiv *function;
	
	function = (struct_glGetVertexAttribiv *)pDatabuf;
	memset(function->params, 0, sizeof(GLint) * 4);

	glGetVertexAttribiv(function->index, function->pname, function->params);

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glGetVertexAttribPointerv(int *pDatabuf)
{
	int ret;
	struct_glGetVertexAttribPointerv *function;
	
	function = (struct_glGetVertexAttribPointerv *)pDatabuf;

	glGetVertexAttribPointerv(function->index, function->pname, &(function->pointer));

	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glHint(int *pDatabuf)
{
	struct_glHint *function;
	
	function = (struct_glHint *)pDatabuf;

	glHint(function->target, function->mode);

	return 0;
}

// this function must return a value with GLbloolean type
int call_glIsBuffer(int *pDatabuf)
{
	int ret;
	struct_glIsBuffer *function;
	GLboolean result;
	
	function = (struct_glIsBuffer *)pDatabuf;

	result = glIsBuffer(function->buffer);

	function->retVal = result;
	
	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

// this function must return a value with GLbloolean type
int call_glIsEnabled(int *pDatabuf)
{
	int ret;
	struct_glIsEnabled *function;
	GLboolean result;
	
	function = (struct_glIsEnabled *)pDatabuf;

	result = glIsEnabled(function->cap);

	function->retVal = result;
	
	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

// this function must return a value with GLbloolean type
int call_glIsFramebuffer(int *pDatabuf)
{
	int ret;
	struct_glIsFramebuffer *function;
	GLboolean result;
	
	function = (struct_glIsFramebuffer *)pDatabuf;

	result = glIsFramebuffer(function->framebuffer);

	function->retVal = result;
	
	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

// this function must return a value with GLbloolean type
int call_glIsProgram(int *pDatabuf)
{
	int ret;
	struct_glIsProgram *function;
	GLboolean result;
	
	function = (struct_glIsProgram *)pDatabuf;

	result = glIsProgram(function->program);

	function->retVal = result;
	
	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

// this function must return a value with GLbloolean type
int call_glIsRenderbuffer(int *pDatabuf)
{
	int ret;
	struct_glIsRenderbuffer *function;
	GLboolean result;
	
	function = (struct_glIsRenderbuffer *)pDatabuf;

	result = glIsRenderbuffer(function->renderbuffer);

	function->retVal = result;
	
	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

// this function must return a value with GLbloolean type
int call_glIsShader(int *pDatabuf)
{
	int ret;
	struct_glIsShader *function;
	GLboolean result;
	
	function = (struct_glIsShader *)pDatabuf;

	result = glIsShader(function->shader);

	function->retVal = result;
	
	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

// this function must return a value with GLbloolean type
int call_glIsTexture(int *pDatabuf)
{
	int ret;
	struct_glIsTexture *function;
	GLboolean result;
	
	function = (struct_glIsTexture *)pDatabuf;

	result = glIsTexture(function->texture);

	function->retVal = result;
	
	if(function->hd.retFlag == RT_REQUEST) {
		function->hd.retFlag = RT_RESPONSE;
		ret = write(dev, function, MAX_BUF_SIZE);
	}

	return 0;
}

int call_glLineWidth(int *pDatabuf)
{
	struct_glLineWidth *function;
	
	function = (struct_glLineWidth *)pDatabuf;

	glLineWidth(function->width);

	return 0;
}

int call_glPixelStorei(int *pDatabuf)
{
	struct_glPixelStorei *function;

	function = (struct_glPixelStorei *)pDatabuf;

	glPixelStorei(function->pname, function->param);

	return 0;
}

int call_glPolygonOffset(int *pDatabuf)
{
	struct_glPolygonOffset *function;

	function = (struct_glPolygonOffset *)pDatabuf;

	glPolygonOffset(function->factor, function->units);

	return 0;
}

int call_glReadPixels(int *pDatabuf)
{
	int ret;
	struct_glReadPixels *function;
	struct_glReadPixels *writebuf;
	void *buf = NULL;
	GLvoid *pixels = NULL;
	GLsizei pixel_data_length = 0;
//	GLsizei pixel_size_byte = 0;
	GLsizei struct_length = sizeof(struct_glReadPixels);
	GLsizei total_length = 0;
	GLsizei ldata_length = 0;
	
	function = (struct_glReadPixels *)pDatabuf;

//	pixel_size_byte = get_pixel_byte(function->type);
//	pixel_buf_length = (function->width * function->height) * pixel_size_byte;

	pixel_data_length = get_bytes_per_image(function->width, function->height,
											function->format, function->type);

	total_length = struct_length + pixel_data_length;
	ldata_length = total_length - COMM_HEAD_SIZE;

	// buffer for pixel data
	pixels = (GLvoid *)malloc(pixel_data_length);
	memset(pixels, 0, pixel_data_length);
	
	glReadPixels(function->x, function->y, function->width, function->height,
			function->format, function->type, pixels);

	// buffer for transmission
	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);
	
	writebuf = (struct_glReadPixels *)buf;

	writebuf->hd.funcID = function->hd.funcID;
	writebuf->hd.retFlag = RT_RESPONSE;
	writebuf->hd.ldata_length = total_length - COMM_HEAD_SIZE;

	writebuf->x = function->x;
	writebuf->y = function->y;
	writebuf->width = function->width;
	writebuf->height = function->height;
	writebuf->format = function->format;
	writebuf->type = function->type;
	writebuf->pixels_buf_offset = struct_length;

	memcpy((buf + writebuf->pixels_buf_offset), pixels, pixel_data_length);

	// Because write size is total_length + MAX_BUF_SIZE, 
	// the write size is always bigger than MAX_BUF_SIZE.
	// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
	ret = write(dev, buf, (total_length + MAX_BUF_SIZE));

	free(buf);
	if(pixels != NULL)
		free(pixels);

	return 0;
}

int call_glReleaseShaderCompiler(int *pDatabuf)
{
	int ret;
	struct_glReleaseShaderCompiler *function;
	typ_common_hd return_hd;
	
	function = (struct_glReleaseShaderCompiler *)pDatabuf;

	glReleaseShaderCompiler();

	return_hd.funcID = function->hd.funcID;
	return_hd.retFlag = RT_RESPONSE;
	ret = write(dev, function, MAX_BUF_SIZE);

	return 0;
}

int call_glRenderbufferStorage(int *pDatabuf)
{
	int ret;
	struct_glRenderbufferStorage *function;
	typ_common_hd return_hd;
	
	function = (struct_glRenderbufferStorage *)pDatabuf;

	glRenderbufferStorage(function->target, function->internalformat, function->width, function->height);

	return_hd.funcID = function->hd.funcID;
	return_hd.retFlag = RT_RESPONSE;
	ret = write(dev, function, MAX_BUF_SIZE);

	return 0;
}

int call_glSampleCoverage(int *pDatabuf)
{
	struct_glSampleCoverage *function;
	
	function = (struct_glSampleCoverage *)pDatabuf;

	glSampleCoverage(function->value, function->invert);

	return 0;
}

int call_glScissor(int *pDatabuf)
{
	struct_glScissor *function;
	
	function = (struct_glScissor *)pDatabuf;

	glScissor(function->x, function->y, function->width, function->height);

	return 0;
}

int call_glShaderBinary(int *pDatabuf)
{
	int ret = 0;
	void *buf = NULL;
	struct_glShaderBinary *function;
	typ_common_hd return_buf;

	GLuint *shaders = NULL;
	GLvoid *binary = NULL;

	GLsizei shaders_length = 0;
	GLsizei total_buf_length = 0;

	function = (struct_glShaderBinary *)pDatabuf;

	total_buf_length = COMM_HEAD_SIZE + function->hd.ldata_length;

	buf = (void*)malloc(total_buf_length);
	memset(buf, 0, total_buf_length);

	// Copy common header
	memcpy(buf, pDatabuf, COMM_HEAD_SIZE);

	function = (struct_glShaderBinary *)buf;

	if(function->hd.ldata_length > 0) {
		// Read large data buffer
		// Because read size is ldata_length + MAX_BUF_SIZE, 
		// the read size is always bigger than MAX_BUF_SIZE.
		// Read function in kernel copies the large data with "read size - MAX_BUF_SIZE"
		ret = read_without_overflow(dev, (buf + COMM_HEAD_SIZE), (function->hd.ldata_length + MAX_BUF_SIZE));

		shaders_length = sizeof(GLuint) * function->n;
		shaders = (GLuint *)malloc(shaders_length);
		memcpy(shaders, (buf + function->shaders_buf_offset), shaders_length);

		binary = (GLvoid *)malloc(function->length);
		memcpy(binary, (buf + function->binary_buf_offset), function->length);
	}
	
	glShaderBinary (function->n, shaders, function->binaryformat, binary, function->length);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	free(buf);
	free(shaders);
	free(binary);

	return 0;
}

int call_glStencilFunc(int *pDatabuf)
{
	int ret;
	struct_glStencilFunc *function;
	typ_common_hd return_buf;

	function = (struct_glStencilFunc *)pDatabuf;

	glStencilFunc(function->func, function->ref, function->mask);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glStencilFuncSeparate(int *pDatabuf)
{
	struct_glStencilFuncSeparate *function;

	function = (struct_glStencilFuncSeparate *)pDatabuf;

	glStencilFuncSeparate(function->face, function->func, function->ref, function->mask);

	return 0;
}

int call_glStencilMask(int *pDatabuf)
{
	int ret;
	struct_glStencilMask *function;
	typ_common_hd return_buf;

	function = (struct_glStencilMask *)pDatabuf;

	glStencilMask(function->mask);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glStencilMaskSeparate(int *pDatabuf)
{
	struct_glStencilMaskSeparate *function;

	function = (struct_glStencilMaskSeparate *)pDatabuf;

	glStencilMaskSeparate(function->face, function->mask);

	return 0;
}

int call_glStencilOp(int *pDatabuf)
{
	int ret;
	struct_glStencilOp *function;
	typ_common_hd return_buf;

	function = (struct_glStencilOp *)pDatabuf;

	glStencilOp(function->fail, function->zfail, function->zpass);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glStencilOpSeparate(int *pDatabuf)
{
	struct_glStencilOpSeparate *function;

	function = (struct_glStencilOpSeparate *)pDatabuf;

	glStencilOpSeparate(function->face, function->fail, function->zfail, function->zpass);

	return 0;
}

int call_glTexImage2D(int *pDatabuf)
{
	int ret = 0, i;
	struct_glTexImage2D *function;
	typ_common_hd return_buf;

	GLvoid *pixels = NULL;
	GLvoid *curr_pixels = NULL;

	GLsizei pixel_data_length = 0;
	GLsizei last_ldata_length = 0;
	GLsizei ldata_count = 0;

	function = (struct_glTexImage2D *)pDatabuf;

	if(function->hd.ldata_length > 0) {
		pixel_data_length = get_bytes_per_image(function->width, function->height,
												function->format, function->type);

		ldata_count = pixel_data_length / MAX_LDATA_SIZE_1M;
		last_ldata_length = pixel_data_length % MAX_LDATA_SIZE_1M;

		pixels = (GLvoid*)malloc(pixel_data_length);
		memset(pixels, 0, pixel_data_length);

		curr_pixels = pixels;
		for(i=0; i<ldata_count; i++) {
			// request "pixels"
			function->hd.retFlag = RT_RESPONSE;
			ret = write(dev, function, COMM_HEAD_SIZE);

			ret = read_without_overflow(dev, curr_pixels, (MAX_LDATA_SIZE_1M + MAX_BUF_SIZE));

			curr_pixels = curr_pixels + MAX_LDATA_SIZE_1M;
		}

		if(last_ldata_length > 0) {
			// request the last data of "pixels"
			function->hd.retFlag = RT_RESPONSE;
			ret = write(dev, function, COMM_HEAD_SIZE);

			ret = read_without_overflow(dev, curr_pixels, (last_ldata_length + MAX_BUF_SIZE));
		}
	}

	glTexImage2D (function->target, function->level, function->internalformat, 
				function->width, function->height, function->border, 
				function->format, function->type, pixels);

	// notify that all data processing is completed
	return_buf.funcID = function->hd.funcID;
	return_buf.retFlag = RT_RESPONSE;
	return_buf.ldata_length = 0;
	ret = write(dev, &return_buf, COMM_HEAD_SIZE);

	if(pixels != NULL)
		free(pixels);

	return 0;
}

int call_glTexParameterf(int *pDatabuf)
{
	int ret;
	struct_glTexParameterf *function;
	typ_common_hd return_buf;

	function = (struct_glTexParameterf *)pDatabuf;

	glTexParameterf(function->target, function->pname, function->param);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glTexParameterfv(int *pDatabuf)
{
	int ret;
	struct_glTexParameterfv *function;
	typ_common_hd return_buf;

	function = (struct_glTexParameterfv *)pDatabuf;

	glTexParameterfv(function->target, function->pname, function->params);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glTexParameteri(int *pDatabuf)
{
	int ret;
	struct_glTexParameteri *function;
	typ_common_hd return_buf;

	function = (struct_glTexParameteri *)pDatabuf;

	glTexParameteri(function->target, function->pname, function->param);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glTexParameteriv(int *pDatabuf)
{
	int ret;
	struct_glTexParameteriv *function;
	typ_common_hd return_buf;

	function = (struct_glTexParameteriv *)pDatabuf;

	glTexParameteriv(function->target, function->pname, function->params);

	if(function->hd.retFlag == RT_REQUEST){
		return_buf.funcID = function->hd.funcID;
		return_buf.retFlag = RT_RESPONSE;
		return_buf.ldata_length = 0;
		ret = write(dev, &return_buf, COMM_HEAD_SIZE);
	}

	return 0;
}

int call_glTexSubImage2D(int *pDatabuf)
{
	int ret = 0, i;
	struct_glTexSubImage2D *function;
	typ_common_hd return_buf;
	GLvoid *pixels = NULL;
	GLvoid *curr_pixels = NULL;

	GLsizei pixel_data_length = 0;
	GLsizei last_ldata_length = 0;
	GLsizei ldata_count = 0;

	function = (struct_glTexSubImage2D *)pDatabuf;

	if(function->hd.ldata_length > 0) {
		pixel_data_length = get_bytes_per_image(function->width, function->height,
												function->format, function->type);

		ldata_count = pixel_data_length / MAX_LDATA_SIZE_1M;
		last_ldata_length = pixel_data_length % MAX_LDATA_SIZE_1M;

		pixels = (GLvoid*)malloc(pixel_data_length);
		memset(pixels, 0, pixel_data_length);

		curr_pixels = pixels;
		for(i=0; i<ldata_count; i++) {
			// request "pixels"
			function->hd.retFlag = RT_RESPONSE;
			ret = write(dev, function, COMM_HEAD_SIZE);

			ret = read_without_overflow(dev, curr_pixels, (MAX_LDATA_SIZE_1M + MAX_BUF_SIZE));

			curr_pixels = curr_pixels + MAX_LDATA_SIZE_1M;
		}

		if(last_ldata_length > 0) {
			// request the last data of "pixels"
			function->hd.retFlag = RT_RESPONSE;
			ret = write(dev, function, COMM_HEAD_SIZE);

			ret = read_without_overflow(dev, curr_pixels, (last_ldata_length + MAX_BUF_SIZE));
		}
	}

	glTexSubImage2D (function->target, function->level, 
				function->xoffset, function->yoffset, function->width, function->height, 
				function->format, function->type, pixels);

	// notify that all data processing is completed
	return_buf.funcID = function->hd.funcID;
	return_buf.retFlag = RT_RESPONSE;
	return_buf.ldata_length = 0;
	ret = write(dev, &return_buf, COMM_HEAD_SIZE);

	if(pixels != NULL)
		free(pixels);

	return 0;
}


/////////// 0604 completion /////////////
int call_glUniform1f(int *pDatabuf)
{
	struct_glUniform1f *function;

	function = (struct_glUniform1f *)pDatabuf;
	
	glUniform1f(function->location, function->x);

	return 0;
}

int call_glUniform1fv(int *pDatabuf)
{
	struct_glUniform1fv *function;
	unsigned int dim = 1;
	unsigned int vertex_cnt;
	GLfloat *vertex;

	function = (struct_glUniform1fv *)pDatabuf;

	vertex_cnt = (dim * function->count);
	vertex = (GLfloat *)malloc(sizeof(GLfloat)*vertex_cnt);
	memcpy(vertex, function->v, sizeof(GLfloat)*vertex_cnt);

	glUniform1fv(function->location, function->count, vertex);

	free(vertex);

	return 0;
}

int call_glUniform1i(int *pDatabuf)
{
	struct_glUniform1i *function;

	function = (struct_glUniform1i *)pDatabuf;
	
	glUniform1i(function->location, function->x);

	return 0;
}

int call_glUniform1iv(int *pDatabuf)
{
	struct_glUniform1iv *function;
	unsigned int dim = 1;
	unsigned int vertex_cnt;
	GLint *vertex;

	function = (struct_glUniform1iv *)pDatabuf;

	vertex_cnt = (dim * function->count);
	vertex = (GLint *)malloc(sizeof(GLint)*vertex_cnt);
	memcpy(vertex, function->v, sizeof(GLint)*vertex_cnt);

	glUniform1iv(function->location, function->count, vertex);

	free(vertex);

	return 0;
}

int call_glUniform2f(int *pDatabuf)
{
	struct_glUniform2f *function;

	function = (struct_glUniform2f *)pDatabuf;
	
	glUniform2f(function->location, function->x, function->y);

	return 0;
}

int call_glUniform2fv(int *pDatabuf)
{
	struct_glUniform2fv *function;
	unsigned int dim = 2;
	unsigned int vertex_cnt;
	GLfloat *vertex;

	function = (struct_glUniform2fv *)pDatabuf;

	vertex_cnt = (dim * function->count);
	vertex = (GLfloat *)malloc(sizeof(GLfloat)*vertex_cnt);
	memcpy(vertex, function->v, sizeof(GLfloat)*vertex_cnt);

	glUniform2fv(function->location, function->count, vertex);

	free(vertex);

	return 0;
}

int call_glUniform2i(int *pDatabuf)
{
	struct_glUniform2i *function;

	function = (struct_glUniform2i *)pDatabuf;
	
	glUniform2i(function->location, function->x, function->y);

	return 0;
}

int call_glUniform2iv(int *pDatabuf)
{
	struct_glUniform2iv *function;
	unsigned int dim = 2;
	unsigned int vertex_cnt;
	GLint *vertex;

	function = (struct_glUniform2iv *)pDatabuf;

	vertex_cnt = (dim * function->count);
	vertex = (GLint *)malloc(sizeof(GLint)*vertex_cnt);
	memcpy(vertex, function->v, sizeof(GLint)*vertex_cnt);

	glUniform2iv(function->location, function->count, vertex);

	free(vertex);

	return 0;
}

int call_glUniform3f(int *pDatabuf)
{
	struct_glUniform3f *function;

	function = (struct_glUniform3f *)pDatabuf;
	
	glUniform3f(function->location, function->x, function->y, function->z);

	return 0;
}

int call_glUniform3fv(int *pDatabuf)
{
	struct_glUniform3fv *function;
	unsigned int dim = 3;
	unsigned int vertex_cnt;
	GLfloat *vertex;

	function = (struct_glUniform3fv *)pDatabuf;

	vertex_cnt = (dim * function->count);
	vertex = (GLfloat *)malloc(sizeof(GLfloat)*vertex_cnt);
	memcpy(vertex, function->v, sizeof(GLfloat)*vertex_cnt);

	glUniform3fv(function->location, function->count, vertex);

	free(vertex);

	return 0;
}

int call_glUniform3i(int *pDatabuf)
{
	struct_glUniform3i *function;

	function = (struct_glUniform3i *)pDatabuf;
	
	glUniform3i(function->location, function->x, function->y, function->z);

	return 0;
}

int call_glUniform3iv(int *pDatabuf)
{
	struct_glUniform3iv *function;
	unsigned int dim = 3;
	unsigned int vertex_cnt;
	GLint *vertex;

	function = (struct_glUniform3iv *)pDatabuf;

	vertex_cnt = (dim * function->count);
	vertex = (GLint *)malloc(sizeof(GLint)*vertex_cnt);
	memcpy(vertex, function->v, sizeof(GLint)*vertex_cnt);

	glUniform3iv(function->location, function->count, vertex);

	free(vertex);

	return 0;
}

int call_glUniform4f(int *pDatabuf)
{
	struct_glUniform4f *function;

	function = (struct_glUniform4f *)pDatabuf;
	
	glUniform4f(function->location, function->x, function->y, function->z, function->w);

	return 0;
}

int call_glUniform4fv(int *pDatabuf)
{
	struct_glUniform4fv *function;
	unsigned int dim = 4;
	unsigned int vertex_cnt;
	GLfloat *vertex;

	function = (struct_glUniform4fv *)pDatabuf;

	vertex_cnt = (dim * function->count);
	vertex = (GLfloat *)malloc(sizeof(GLfloat)*vertex_cnt);
	memcpy(vertex, function->v, sizeof(GLfloat)*vertex_cnt);

	glUniform4fv(function->location, function->count, vertex);

	free(vertex);

	return 0;
}

int call_glUniform4i(int *pDatabuf)
{
	struct_glUniform4i *function;

	function = (struct_glUniform4i *)pDatabuf;
	
	glUniform4i(function->location, function->x, function->y, function->z, function->w);

	return 0;
}

int call_glUniform4iv(int *pDatabuf)
{
	struct_glUniform4iv *function;
	unsigned int dim = 4;
	unsigned int vertex_cnt;
	GLint *vertex;

	function = (struct_glUniform4iv *)pDatabuf;

	vertex_cnt = (dim * function->count);
	vertex = (GLint *)malloc(sizeof(GLint)*vertex_cnt);
	memcpy(vertex, function->v, sizeof(GLint)*vertex_cnt);

	glUniform4iv(function->location, function->count, vertex);

	free(vertex);

	return 0;
}

int call_glUniformMatrix2fv(int *pDatabuf)
{
	struct_glUniformMatrix2fv *function;
	function = (struct_glUniformMatrix2fv *)pDatabuf;

	glUniformMatrix2fv(function->location, function->count, function->transpose, function->value);

	return 0;

}

int call_glUniformMatrix3fv(int *pDatabuf)
{
	struct_glUniformMatrix3fv *function;
	function = (struct_glUniformMatrix3fv *)pDatabuf;

	glUniformMatrix3fv(function->location, function->count, function->transpose, function->value);

	return 0;

}

int call_glValidateProgram(int *pDatabuf)
{
	struct_glValidateProgram *function;
	function = (struct_glValidateProgram *)pDatabuf;

	glValidateProgram(function->program);

	return 0;
}

int call_glVertexAttrib1f(int *pDatabuf)
{
	struct_glVertexAttrib1f *function;
	function = (struct_glVertexAttrib1f *)pDatabuf;

	glVertexAttrib1f(function->indx, function->x);

	return 0;
}

int call_glVertexAttrib1fv(int *pDatabuf)
{
	struct_glVertexAttrib1fv *function;
	unsigned int dim = 1;
	GLfloat *values;

	function = (struct_glVertexAttrib1fv *)pDatabuf;

	values = (GLfloat *)malloc(sizeof(GLfloat)*dim);
	memcpy(values, function->values, sizeof(GLfloat)*dim);

	glVertexAttrib1fv(function->indx, values);

	free(values);

	return 0;
}

int call_glVertexAttrib2f(int *pDatabuf)
{
	struct_glVertexAttrib2f *function;
	function = (struct_glVertexAttrib2f *)pDatabuf;

	glVertexAttrib2f(function->indx, function->x, function->y);

	return 0;
}

int call_glVertexAttrib2fv(int *pDatabuf)
{
	struct_glVertexAttrib2fv *function;
	unsigned int dim = 2;
	GLfloat *values;

	function = (struct_glVertexAttrib2fv *)pDatabuf;

	values = (GLfloat *)malloc(sizeof(GLfloat)*dim);
	memcpy(values, function->values, sizeof(GLfloat)*dim);

	glVertexAttrib2fv(function->indx, values);

	free(values);

	return 0;
}

int call_glVertexAttrib3f(int *pDatabuf)
{
	struct_glVertexAttrib3f *function;
	function = (struct_glVertexAttrib3f *)pDatabuf;

	glVertexAttrib3f(function->indx, function->x, function->y, function->z);

	return 0;
}

int call_glVertexAttrib3fv(int *pDatabuf)
{
	struct_glVertexAttrib3fv *function;
	unsigned int dim = 3;
	GLfloat *values;

	function = (struct_glVertexAttrib3fv *)pDatabuf;

	values = (GLfloat *)malloc(sizeof(GLfloat)*dim);
	memcpy(values, function->values, sizeof(GLfloat)*dim);

	glVertexAttrib3fv(function->indx, values);

	free(values);

	return 0;
}

int call_glVertexAttrib4f(int *pDatabuf)
{
	struct_glVertexAttrib4f *function;
	function = (struct_glVertexAttrib4f *)pDatabuf;

	glVertexAttrib4f(function->indx, function->x, function->y, function->z, function->w);

	return 0;
}

int call_glVertexAttrib4fv(int *pDatabuf)
{
	struct_glVertexAttrib4fv *function;
	unsigned int dim = 4;
	GLfloat *values;

	function = (struct_glVertexAttrib4fv *)pDatabuf;

	values = (GLfloat *)malloc(sizeof(GLfloat)*dim);
	memcpy(values, function->values, sizeof(GLfloat)*dim);

	glVertexAttrib4fv(function->indx, values);

	free(values);

	return 0;
}


