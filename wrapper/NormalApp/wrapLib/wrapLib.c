#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "wrapLib_struct.h"

int dev = -1;
int Ret = -1;

char wbuf[MAX_BUF_SIZE] ={0, };
char rbuf[MAX_BUF_SIZE] ={0, };

struct space_list *unfreed = NULL;
struct space_list *unfreed_tail = NULL;

void add_to_unfreed_list(struct space_list *header, struct space_list *tail, void *addr)
{
	struct space_list *tmp = NULL;
	
	if(header == NULL) {
		header = (struct space_list *)malloc(sizeof(struct space_list));
		tail = header;
		tail->next = NULL;
	}
	else {
		tmp = (struct space_list *)malloc(sizeof(struct space_list));
		tmp->buf = addr;
		tmp->next = NULL;

		tail->next = tmp;
		tail = tmp;
	}
}

void free_unfreed_list(struct space_list *header)
{
	struct space_list *tmp = NULL;
	
	while(header != NULL) {
		tmp = header;
		header = header->next;

		free(tmp->buf);
		free(tmp);
	}
}

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

int read_from_shmem(int filp, void *buf, int count)
{
	int ret = -1;

	do {
		ret = read(filp, buf, count);

		if(ret > 0)
			break;
		else
			usleep(1000);
	} while(ret <= 0);

	return ret;
}

void glClear(GLbitfield mask)
{

	str_glClear *writebuf;

	writebuf = (str_glClear*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_glClear;
	writebuf->hd.retFlag = 0;

	writebuf->mask = mask;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glViewport (GLint x, GLint y, GLsizei width, GLsizei height) 
{
	str_glViewport *writebuf;

	writebuf = (str_glViewport*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_glViewport;
	writebuf->hd.retFlag = 0;

	writebuf->x = x;
	writebuf->y = y;
	writebuf->width = width;
	writebuf->height = height;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) 
{
	str_glUniformMatrix4fv *writebuf;

	writebuf = (str_glUniformMatrix4fv*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_glUniformMatrix4fv;
	writebuf->hd.retFlag = 0;

	writebuf->location = location;
	writebuf->count = count;
	writebuf->transpose = transpose;

	memset(writebuf->value, 0, sizeof(GLfloat)*16);
	if(value != NULL) {
		memcpy(writebuf->value, value, sizeof(GLfloat)*16);
	}
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

/*
 * Set positions of vertex for a texture
 */
void glVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr) 
{
	str_glVertexAttribPointer *writebuf;

	writebuf = (str_glVertexAttribPointer*)wbuf;
	memset(writebuf, 0, MAX_BUF_SIZE);

	writebuf->hd.funcID = ID_glVertexAttribPointer;
	writebuf->hd.retFlag = RT_NONE;

	writebuf->indx = indx;
	writebuf->size = size;
	writebuf->type = type;
	writebuf->normalized = normalized;
	writebuf->stride = stride;

	if(ptr != NULL) {
		memcpy(writebuf->ptr, ptr, PARAM_SIZE(glVertexAttribPointer));
	}
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glEnableVertexAttribArray (GLuint index) 
{
	str_glEnableVertexAttribArray *writebuf;

	writebuf = (str_glEnableVertexAttribArray*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_glEnableVertexAttribArray;
	writebuf->hd.retFlag = 0;

	writebuf->index = index;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glDrawArrays (GLenum mode, GLint first, GLsizei count) 
{
	str_glDrawArrays *writebuf;

	writebuf = (str_glDrawArrays*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_glDrawArrays;
	writebuf->hd.retFlag = 0;

	writebuf->mode = mode;
	writebuf->first = first;
	writebuf->count = count;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}
void glDisableVertexAttribArray (GLuint index) 
{
	
	str_glDisableVertexAttribArray *writebuf;

	writebuf = (str_glDisableVertexAttribArray*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_glDisableVertexAttribArray;
	writebuf->hd.retFlag = 0;

	writebuf->index = index;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

GLuint glCreateShader(GLenum shaderType)
{
	str_glCreateShader *writebuf;
	str_glCreateShader *readbuf;

	writebuf = (str_glCreateShader*)wbuf;
	readbuf = (str_glCreateShader*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));
	
	writebuf->hd.funcID = ID_glCreateShader;
	writebuf->hd.retFlag = 1;

	writebuf->shaderType = shaderType;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_glCreateShader;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	GLuint type = readbuf->retVal;

	return type;
}

void glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint *length)
{
	int Ret, i;
	void *addr = NULL;
	GLchar *cur_string_buf = NULL;
	str_glShaderSource *writebuf;
	typ_common_hd *readbuf;
	
	unsigned int struct_length = 0;
	unsigned int string_length = 0;
	unsigned int total_length = 0;
	unsigned int len_buf_length = 0;

	if(*string == NULL)
		return ;

	struct_length = sizeof(str_glShaderSource);
	if(length == NULL) {
		len_buf_length = 0;
		for(i=0; i<count; i++)
			string_length = strlen(string[i]) + 1;	// include null terminator
	}
	else {
		len_buf_length = sizeof(GLint) * count;
		for(i=0; i<count; i++)
			string_length += length[i] + 1;	// include null terminator
	}
	total_length = struct_length + string_length + len_buf_length;

	addr = (void*)malloc(total_length);
	memset(addr, 0, total_length);

	writebuf = (str_glShaderSource *)addr;
	
	writebuf->hd.funcID = ID_glShaderSource;
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->hd.ldata_length = total_length - COMM_HEAD_SIZE;
	writebuf->shader = shader;
	writebuf->count = count;

	// copy a length array
	if(length != NULL) {
		writebuf->len_buf_offset = struct_length;
		writebuf->str_buf_offset = writebuf->len_buf_offset + len_buf_length;
		memcpy((addr + writebuf->len_buf_offset), length, len_buf_length);
	}
	else {
		writebuf->len_buf_offset = 0;
		writebuf->str_buf_offset = struct_length;
	}

	// copy strings
	cur_string_buf = addr + writebuf->str_buf_offset;
	if(length != NULL) {
		for(i=0; i<count; i++) {
			// include null terminator
			memcpy(cur_string_buf, string[i], length[i]);
			cur_string_buf = cur_string_buf + length[i];
			*cur_string_buf = '\0';
			cur_string_buf = cur_string_buf + 1;
		}
	}
	else {
		for(i=0; i<count; i++) {
			// include null terminator
			strcpy(cur_string_buf, string[i]);
//			memcpy(cur_string_buf, string[i], strlen(string[i]));
			cur_string_buf = cur_string_buf + strlen(string[i]) + 1;
		}
	}

	// Because write size is total_length + MAX_BUF_SIZE, 
	// the write size is always bigger than MAX_BUF_SIZE.
	// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
	Ret = write(dev, addr, (total_length + MAX_BUF_SIZE));
	call_log(Ret);

	readbuf = (typ_common_hd*)rbuf;
	readbuf->funcID = ID_glShaderSource;

	// Wait to empty the ldata_buf, until secure world read the data
	Ret = read_from_shmem(dev, readbuf, COMM_HEAD_SIZE);

	free(addr);

	return ;
}

void glCompileShader(GLuint shader)
{
	str_glCompileShader *writebuf;

	writebuf = (str_glCompileShader*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = ID_glCompileShader;
	writebuf->hd.retFlag = 0;

	writebuf->shader = shader;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
	str_glGetShaderiv *writebuf;
	str_glGetShaderiv *readbuf;

	writebuf = (str_glGetShaderiv*)wbuf;
	readbuf = (str_glGetShaderiv*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_glGetShaderiv;
	writebuf->hd.retFlag = 1;

	writebuf->shader = shader;
	writebuf->pname = pname;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_glGetShaderiv;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	if(params != NULL)
//		*params = readbuf->re_params;
		memcpy(params, &(readbuf->re_params), sizeof(GLint));
}

void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
	str_glGetShaderInfoLog *writebuf;
	str_glGetShaderInfoLog *readbuf;

	writebuf = (str_glGetShaderInfoLog*)wbuf;
	readbuf = (str_glGetShaderInfoLog*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_glGetShaderInfoLog;
	writebuf->hd.retFlag = 1;

	writebuf->shader = shader;
	if(maxLength > PARAM_SIZE(glGetShaderInfoLog))
		writebuf->maxLength = PARAM_SIZE(glGetShaderInfoLog);
	else
		writebuf->maxLength = maxLength;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_glGetShaderInfoLog;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	if(length != NULL)
		*length = readbuf->re_length;
	if(infoLog != NULL)
		memcpy(infoLog, readbuf->re_infoLog, *length);
}


GLuint glCreateProgram(void)
{
	str_glCreateProgram *writebuf;
	str_glCreateProgram *readbuf;

	writebuf = (str_glCreateProgram*)wbuf;
	readbuf = (str_glCreateProgram*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));
	
	writebuf->hd.funcID = ID_glCreateProgram;
	writebuf->hd.retFlag = 1;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_glCreateProgram;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	GLuint ret_program = readbuf->retVal;
	return ret_program;
}

void glAttachShader(GLuint program, GLuint shader)
{
	str_glAttachShader *writebuf;

	writebuf = (str_glAttachShader*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = ID_glAttachShader;
	writebuf->hd.retFlag = 0;

	writebuf->program = program;
	writebuf->shader = shader;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glBindAttribLocation(GLuint program, GLuint index, const GLchar *name)
{
	str_glBindAttribLocation *writebuf;

	writebuf = (str_glBindAttribLocation*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_glBindAttribLocation;
	writebuf->hd.retFlag = 0;

	writebuf->program = program;
	writebuf->index = index;
	
	memset(writebuf->name, 0, PARAM_SIZE(glBindAttribLocation));
	if(name != NULL)
		memcpy(writebuf->name, name, PARAM_SIZE(glBindAttribLocation));
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}
void glLinkProgram(GLuint program)
{
	str_glLinkProgram *writebuf;

	writebuf = (str_glLinkProgram*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_glLinkProgram;
	writebuf->hd.retFlag = 0;

	writebuf->program = program;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}
void glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
	str_glGetProgramiv *writebuf;
	str_glGetProgramiv *readbuf;

	writebuf = (str_glGetProgramiv*)wbuf;
	readbuf = (str_glGetProgramiv*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_glGetProgramiv;
	writebuf->hd.retFlag = 1;

	writebuf->program = program;
	writebuf->pname = pname;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_glGetProgramiv;

	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	if(params != NULL)
		*params = readbuf->re_params;
}

void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
	str_glGetProgramInfoLog *writebuf;
	str_glGetProgramInfoLog *readbuf;

	writebuf = (str_glGetProgramInfoLog*)wbuf;
	readbuf = (str_glGetProgramInfoLog*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = ID_glGetProgramInfoLog;
	writebuf->hd.retFlag = 1;

	writebuf->program = program;
	if(maxLength > PARAM_SIZE(glGetProgramInfoLog))
		writebuf->maxLength = PARAM_SIZE(glGetProgramInfoLog);
	else
		writebuf->maxLength = maxLength;

	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);

	readbuf->hd.funcID = ID_glGetProgramInfoLog;
	
	Ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	if(length != NULL)
		*length = readbuf->re_length;
	if(infoLog != NULL)
		memcpy(infoLog, readbuf->re_infoLog, *length);
}

GLint glGetUniformLocation(GLuint program, const GLchar *name)
{
	int ret;
	void *buf = NULL;
	GLint ret_location;
	str_glGetUniformLocation *writebuf;
	str_glGetUniformLocation *readbuf;
	
	unsigned int struct_length = sizeof(str_glGetUniformLocation);
	unsigned int name_length = strlen(name);
	unsigned int total_length = struct_length + name_length + 1;		// include null terminator

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	writebuf = (str_glGetUniformLocation *)buf;

	writebuf->hd.funcID = func_ID(glGetUniformLocation);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->program = program;
	writebuf->size = name_length;
	writebuf->name_buf_offset = struct_length;
	// include null terminator
	memcpy((buf + writebuf->name_buf_offset), name, name_length + 1);

	if(total_length > MAX_BUF_SIZE) {
		writebuf->hd.ldata_length = total_length - COMM_HEAD_SIZE;
		ret= write(dev, buf, (total_length + MAX_BUF_SIZE));
	}
	else {
		ret= write(dev, writebuf, total_length);
	}
	call_log(ret);	

	readbuf = (str_glGetUniformLocation *)rbuf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(glGetUniformLocation);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	ret_location = readbuf->retVal;
	
	if(buf != NULL)
		free(buf);
	
	return ret_location;
}
void glDeleteShader(GLuint shader)
{
	str_glDeleteShader *writebuf;

	writebuf = (str_glDeleteShader*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_glDeleteShader;
	writebuf->hd.retFlag = 0;

	writebuf->shader = shader;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}
void glUseProgram(GLuint program)
{
	str_glUseProgram *writebuf;

	writebuf = (str_glUseProgram*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = ID_glUseProgram;
	writebuf->hd.retFlag = 0;

	writebuf->program = program;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}
void glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) 
{
	str_glClearColor *writebuf;

	writebuf = (str_glClearColor*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = ID_glClearColor;
	writebuf->hd.retFlag = 0;

	writebuf->red = red;
	writebuf->blue = blue;
	writebuf->green = green;
	writebuf->alpha = alpha;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}
void glEnable (GLenum cap) 
{
	int ret;
	str_glEnable *writebuf;
	typ_common_hd readhd;

	writebuf = (str_glEnable*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glEnable);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->cap = cap;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glEnable);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

///////////////////////////////
void glActiveTexture (GLenum texture)
{
	int ret;
	struct_glActiveTexture *writebuf;

	writebuf = (struct_glActiveTexture*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glActiveTexture);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->texture= texture;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glBindBuffer (GLenum target, GLuint buffer)
{
	int ret;
	struct_glBindBuffer *writebuf;

	writebuf = (struct_glBindBuffer*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glBindBuffer);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->target = target;
	writebuf->buffer = buffer;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glBindFramebuffer (GLenum target, GLuint framebuffer)
{
	int ret;
	struct_glBindFramebuffer *writebuf;

	writebuf = (struct_glBindFramebuffer*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glBindFramebuffer);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->target = target;
	writebuf->framebuffer = framebuffer;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glBindRenderbuffer (GLenum target, GLuint renderbuffer)
{
	int ret;
	struct_glBindRenderbuffer *writebuf;

	writebuf = (struct_glBindRenderbuffer*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glBindRenderbuffer);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->target = target;
	writebuf->renderbuffer = renderbuffer;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glBindTexture (GLenum target, GLuint texture)
{
	int ret;
	struct_glBindTexture *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glBindTexture*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glBindTexture);
	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->target = target;
	writebuf->texture = texture;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glBindTexture);

	// Wait to empty the ldata_buf, until secure world read the data
	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glBlendColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	int ret;
	struct_glBlendColor *writebuf;

	writebuf = (struct_glBlendColor*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glBlendColor);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->red = red;
	writebuf->green = green;
	writebuf->blue = blue;
	writebuf->alpha = alpha;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glBlendEquation ( GLenum mode )
{
	int ret;
	struct_glBlendEquation *writebuf;

	writebuf = (struct_glBlendEquation*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glBlendEquation);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->mode = mode;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glBlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha)
{
	int ret;
	struct_glBlendEquationSeparate *writebuf;

	writebuf = (struct_glBlendEquationSeparate*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glBlendEquationSeparate);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->modeRGB = modeRGB;
	writebuf->modeAlpha = modeAlpha;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glBlendFunc (GLenum sfactor, GLenum dfactor)
{
	int ret;
	struct_glBlendFunc *writebuf;

	writebuf = (struct_glBlendFunc*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glBlendFunc);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->sfactor = sfactor;
	writebuf->dfactor = dfactor;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glBlendFuncSeparate (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
	int ret;
	struct_glBlendFuncSeparate *writebuf;

	writebuf = (struct_glBlendFuncSeparate*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glBlendFuncSeparate);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->srcRGB = srcRGB;
	writebuf->dstRGB = dstRGB;
	writebuf->srcAlpha = srcAlpha;
	writebuf->dstAlpha = dstAlpha;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glBufferData (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
{
	int ret;
	void *buf = NULL;
	struct_glBufferData *writebuf;
	typ_common_hd *readbuf;
	
	GLsizeiptr struct_length = 0;
	GLsizeiptr data_length = 0;
	GLsizeiptr total_length = 0;

	struct_length = sizeof(struct_glBufferData);
	if(data == NULL || size == 0)
		data_length = 0;
	else
		data_length = size;
	total_length = struct_length + data_length;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	writebuf = (struct_glBufferData *)buf;
	
	writebuf->hd.funcID = func_ID(glBufferData);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->size = data_length;
	writebuf->data_buf_offset = struct_length;
	writebuf->usage = usage;

	if(data != NULL) {
		writebuf->hd.ldata_length = data_length;
		memcpy((buf + writebuf->data_buf_offset), data, size);
		// Because write size is total_length + MAX_BUF_SIZE, 
		// the write size is always bigger than MAX_BUF_SIZE.
		// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
		ret = write(dev, buf, (total_length + MAX_BUF_SIZE));
	}
	else {
		ret = write(dev, buf, total_length);
	}
	call_log(ret);

	readbuf = (typ_common_hd*)rbuf;
	readbuf->funcID = func_ID(glBufferData);

	// Wait to empty the ldata_buf, until secure world read the data
	Ret = read_from_shmem(dev, readbuf, COMM_HEAD_SIZE);

	free(buf);
}

void glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
	int ret;
	void *buf = NULL;
	struct_glBufferSubData *writebuf;
	typ_common_hd *readbuf;
	
	GLsizeiptr struct_length = 0;
	GLsizeiptr data_length = 0;
	GLsizeiptr total_length = 0;

	struct_length = sizeof(struct_glBufferSubData);
	if(data == NULL || size == 0)
		data_length = 0;
	else
		data_length = size;
	total_length = struct_length + data_length;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	writebuf = (struct_glBufferSubData *)buf;
	
	writebuf->hd.funcID = func_ID(glBufferSubData);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->offset = offset;
	writebuf->size = data_length;
	writebuf->data_buf_offset = struct_length;

	if(data != NULL) {
		writebuf->hd.ldata_length = data_length;
		memcpy((buf + writebuf->data_buf_offset), data, size);
		// Because write size is total_length + MAX_BUF_SIZE, 
		// the write size is always bigger than MAX_BUF_SIZE.
		// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
		ret = write(dev, buf, (total_length + MAX_BUF_SIZE));
	}
	else {
		ret = write(dev, buf, total_length);
	}
	call_log(ret);

	readbuf = (typ_common_hd*)rbuf;
	readbuf->funcID = func_ID(glBufferSubData);

	// Wait to empty the ldata_buf, until secure world read the data
	Ret = read_from_shmem(dev, readbuf, COMM_HEAD_SIZE);

	free(buf);
}

GLenum glCheckFramebufferStatus (GLenum target)
{
	int ret;
	struct_glCheckFramebufferStatus *writebuf;
	struct_glCheckFramebufferStatus *readbuf;
	GLenum status;

	writebuf = (struct_glCheckFramebufferStatus*)wbuf;
	readbuf = (struct_glCheckFramebufferStatus*)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));
	
	writebuf->hd.funcID = func_ID(glCheckFramebufferStatus);
	writebuf->hd.retFlag = RT_REQUEST;

	writebuf->target = target;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf->hd.funcID = func_ID(glCheckFramebufferStatus);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	status = readbuf->retVal;

	return status;
}

void glClearDepthf (GLclampf depth)
{
	int ret;
	struct_glClearDepthf *writebuf;

	writebuf = (struct_glClearDepthf*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glClearDepthf);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->depth = depth;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glClearStencil (GLint s)
{
	int ret;
	struct_glClearStencil *writebuf;

	writebuf = (struct_glClearStencil*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glClearStencil);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->s = s;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	int ret;
	struct_glColorMask *writebuf;

	writebuf = (struct_glColorMask*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glColorMask);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->red = red;
	writebuf->green = green;
	writebuf->blue = blue;
	writebuf->alpha = alpha;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glCompressedTexImage2D (GLenum target, 
								GLint level, GLenum internalformat, 
								GLsizei width, GLsizei height, 
								GLint border, GLsizei imageSize, 
								const GLvoid* data)
{
	int ret;
	void *buf = NULL;
	struct_glCompressedTexImage2D *writebuf;
	typ_common_hd *readbuf;
	
	GLsizei struct_length = 0;
	GLsizei data_length = 0;
	GLsizei total_length = 0;

	struct_length = sizeof(struct_glCompressedTexImage2D);
	if(data == NULL || imageSize == 0)
		return ;
	else
		data_length = imageSize;
	
	total_length = struct_length + data_length;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	writebuf = (struct_glCompressedTexImage2D *)buf;

	writebuf->hd.funcID = func_ID(glCompressedTexImage2D);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->level = level;
	writebuf->internalformat = internalformat;
	writebuf->width = width;
	writebuf->height = height;
	writebuf->border = border;
	writebuf->imageSize = data_length;
	writebuf->data_buf_offset = struct_length;

	writebuf->hd.ldata_length = data_length;
	memcpy((buf + writebuf->data_buf_offset), data, data_length);

	// Because write size is total_length + MAX_BUF_SIZE, 
	// the write size is always bigger than MAX_BUF_SIZE.
	// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
	ret = write(dev, buf, (total_length + MAX_BUF_SIZE));
	call_log(ret);

	readbuf = (typ_common_hd*)rbuf;
	readbuf->funcID = func_ID(glCompressedTexImage2D);

	// Wait to empty the ldata_buf, until secure world read the data
	Ret = read_from_shmem(dev, readbuf, COMM_HEAD_SIZE);

	free(buf);
}

void glCompressedTexSubImage2D (GLenum target, 
								GLint level, GLint xoffset, GLint yoffset, 
								GLsizei width, GLsizei height, GLenum format, 
								GLsizei imageSize, const GLvoid* data)
{
	int ret;
	void *buf = NULL;
	struct_glCompressedTexSubImage2D *writebuf;
	typ_common_hd *readbuf;
	
	GLsizei struct_length = 0;
	GLsizei data_length = 0;
	GLsizei total_length = 0;

	struct_length = sizeof(struct_glCompressedTexSubImage2D);
	if(data == NULL || imageSize == 0)
		return ;
	else
		data_length = imageSize;
	
	total_length = struct_length + data_length;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	writebuf = (struct_glCompressedTexSubImage2D *)buf;

	writebuf->hd.funcID = func_ID(glCompressedTexSubImage2D);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->level = level;
	writebuf->xoffset = xoffset;
	writebuf->yoffset = yoffset;
	writebuf->width = width;
	writebuf->height = height;
	writebuf->format = format;
	writebuf->imageSize = data_length;
	writebuf->data_buf_offset = struct_length;

	writebuf->hd.ldata_length = data_length;
	memcpy((buf + writebuf->data_buf_offset), data, data_length);

	// Because write size is total_length + MAX_BUF_SIZE, 
	// the write size is always bigger than MAX_BUF_SIZE.
	// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
	ret = write(dev, buf, (total_length + MAX_BUF_SIZE));
	call_log(ret);

	readbuf = (typ_common_hd*)rbuf;
	readbuf->funcID = func_ID(glCompressedTexSubImage2D);

	// Wait to empty the ldata_buf, until secure world read the data
	Ret = read_from_shmem(dev, readbuf, COMM_HEAD_SIZE);

	free(buf);
}

void glCopyTexImage2D (GLenum target, GLint level, 
						GLenum internalformat, GLint x, GLint y, 
						GLsizei width, GLsizei height, GLint border)
{
	int ret;
	struct_glCopyTexImage2D *writebuf;
	
	writebuf = (struct_glCopyTexImage2D *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glCopyTexImage2D);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->target = target;
	writebuf->level = level;
	writebuf->internalformat = internalformat;
	writebuf->x = x;
	writebuf->y = y;
	writebuf->width = width;
	writebuf->height = height;
	writebuf->border = border;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glCopyTexSubImage2D (GLenum target, GLint level, 
							GLint xoffset, GLint yoffset, GLint x, GLint y, 
							GLsizei width, GLsizei height)
{
	int ret;
	struct_glCopyTexSubImage2D *writebuf;

	writebuf = (struct_glCopyTexSubImage2D *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glCopyTexSubImage2D);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->target = target;
	writebuf->level = level;
	writebuf->xoffset = xoffset;
	writebuf->yoffset = yoffset;
	writebuf->x = x;
	writebuf->y = y;
	writebuf->width = width;
	writebuf->height = height;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glCullFace (GLenum mode)
{
	int ret;
	struct_glCullFace *writebuf;

	writebuf = (struct_glCullFace *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glCullFace);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->mode = mode;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glDeleteBuffers (GLsizei n, const GLuint* buffers)
{
	int ret;
	struct_glDeleteBuffers *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glDeleteBuffers *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glDeleteBuffers);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->n = n;
	if(buffers != NULL)
		memcpy(writebuf->buffers, buffers, sizeof(GLuint)*n);

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glDeleteBuffers);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glDeleteFramebuffers (GLsizei n, const GLuint* framebuffers)
{
	int ret;
	struct_glDeleteFramebuffers *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glDeleteFramebuffers *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glDeleteFramebuffers);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->n = n;
	if(framebuffers != NULL)
		memcpy(writebuf->framebuffers, framebuffers, sizeof(GLuint)*n);

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glDeleteFramebuffers);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glDeleteProgram (GLuint program)
{
	int ret;
	struct_glDeleteProgram *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glDeleteProgram *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glDeleteProgram);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->program = program;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glDeleteProgram);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glDeleteRenderbuffers (GLsizei n, const GLuint* renderbuffers)
{
	int ret;
	struct_glDeleteRenderbuffers *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glDeleteRenderbuffers *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glDeleteRenderbuffers);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->n = n;
	if(renderbuffers != NULL)
		memcpy(writebuf->renderbuffers, renderbuffers, sizeof(GLuint)*n);

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glDeleteRenderbuffers);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glDeleteTextures (GLsizei n, const GLuint* textures)
{
	int ret;
	struct_glDeleteTextures *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glDeleteTextures *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glDeleteTextures);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->n = n;
	if(textures != NULL)
		memcpy(writebuf->textures, textures, sizeof(GLuint)*n);

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glDeleteTextures);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glDepthFunc (GLenum func)
{
	int ret;
	struct_glDepthFunc *writebuf;

	writebuf = (struct_glDepthFunc *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glDepthFunc);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->func = func;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glDepthMask (GLboolean flag)
{
	int ret;
	struct_glDepthMask *writebuf;

	writebuf = (struct_glDepthMask *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glDepthMask);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->flag = flag;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glDepthRangef (GLclampf zNear, GLclampf zFar)
{
	int ret;
	struct_glDepthRangef *writebuf;

	writebuf = (struct_glDepthRangef *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glDepthRangef);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->zNear = zNear;
	writebuf->zFar = zFar;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glDetachShader (GLuint program, GLuint shader)
{
	int ret;
	struct_glDetachShader *writebuf;

	writebuf = (struct_glDetachShader *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glDetachShader);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->program = program;
	writebuf->shader = shader;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glDisable (GLenum cap)
{
	int ret;
	struct_glDisable *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glDisable *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glDisable);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->cap = cap;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glDisable);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glDrawElements (GLenum mode, GLsizei count, 
					GLenum type, const GLvoid* indices)
{
	int ret, i=0;
	void *buf = NULL;
	const GLvoid *cur_indices = NULL;
	struct_glDrawElements *writebuf;
	typ_common_hd readhd;
	
	GLsizei struct_length = 0;
	GLsizei data_length = 0;

	GLsizei one_ldata_length = 0;
	GLsizei ldata_count = 0;
	GLsizei last_ldata_length = 0;

	struct_length = sizeof(struct_glDrawElements);
	if(count < 0)
		return ;
	else {
		if(type == GL_UNSIGNED_BYTE)
			data_length = count * sizeof(unsigned char);
		else if(type == GL_UNSIGNED_SHORT)
			data_length = count * sizeof(unsigned short);
		else
			return ;
	}

	ldata_count = data_length / MAX_LDATA_SIZE_1M;
	last_ldata_length = data_length % MAX_LDATA_SIZE_1M;

	writebuf = (struct_glDrawElements *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glDrawElements);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->mode = mode;
	writebuf->count = count;
	writebuf->type = type;

	if(indices != NULL) {
		writebuf->indices_buf_offset = struct_length;
		writebuf->hd.ldata_length = data_length;

		// send arguments except "pixels"
		ret = write(dev, writebuf, struct_length);

		// all divided "pixels" data have to include the common header to identify the function id
		one_ldata_length = COMM_HEAD_SIZE + MAX_LDATA_SIZE_1M;
		buf = (void*)malloc(one_ldata_length);
		memset(buf, 0, one_ldata_length);

		ret = ioctl(dev, IMMEDIATE_SEND_START);
		for(i=0; i<ldata_count; i++) {
			// wait for request for remained data
			readhd.funcID = func_ID(glDrawElements);
			ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);

			cur_indices = indices + (MAX_LDATA_SIZE_1M * i);
			memcpy(buf, writebuf, COMM_HEAD_SIZE);
			memcpy((buf+COMM_HEAD_SIZE), cur_indices, MAX_LDATA_SIZE_1M);

			ret = write(dev, buf, (one_ldata_length + MAX_BUF_SIZE));
		}

		if(last_ldata_length > 0) {
			// wait for request for remained data
			readhd.funcID = func_ID(glDrawElements);
			ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);

			cur_indices = indices + (MAX_LDATA_SIZE_1M * i);
			memcpy(buf, writebuf, COMM_HEAD_SIZE);
			memcpy((buf+COMM_HEAD_SIZE), cur_indices, last_ldata_length);

			ret = write(dev, buf, (last_ldata_length + COMM_HEAD_SIZE + MAX_BUF_SIZE));
		}
		ret = ioctl(dev, IMMEDIATE_SEND_STOP);
	}
	else {
		writebuf->indices_buf_offset = 0;
		writebuf->hd.ldata_length = 0;

		ret = write(dev, writebuf, struct_length);
	}
	call_log(ret);

	readhd.funcID = func_ID(glDrawElements);
	// Wait to empty the ldata_buf, until secure world read the data
	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);

	if(buf != NULL)
		free(buf);
}

void glFinish (void)
{
	int ret;
	struct_glFinish *writebuf;

	writebuf = (struct_glFinish *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glFinish);
	writebuf->hd.retFlag = RT_NONE;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glFlush (void)
{
	int ret;
	struct_glFlush *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glFlush *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glFlush);
	writebuf->hd.retFlag = RT_REQUEST;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glFlush);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glFramebufferRenderbuffer (GLenum target, GLenum attachment, 
							GLenum renderbuffertarget, GLuint renderbuffer)
{
	int ret;
	struct_glFramebufferRenderbuffer *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glFramebufferRenderbuffer *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glFramebufferRenderbuffer);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->attachment = attachment;
	writebuf->renderbuffertarget = renderbuffertarget;
	writebuf->renderbuffer = renderbuffer;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glFramebufferRenderbuffer);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glFramebufferTexture2D (GLenum target, GLenum attachment, 
							GLenum textarget, GLuint texture, GLint level)
{
	int ret;
	struct_glFramebufferTexture2D *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glFramebufferTexture2D *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glFramebufferTexture2D);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->attachment = attachment;
	writebuf->textarget = textarget;
	writebuf->texture = texture;
	writebuf->level = level;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glFramebufferTexture2D);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glFrontFace (GLenum mode)
{
	int ret;
	struct_glFrontFace *writebuf;

	writebuf = (struct_glFrontFace *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glFrontFace);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->mode = mode;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glGenBuffers (GLsizei n, GLuint* buffers)
{
	int ret;
	struct_glGenBuffers *writebuf;
	struct_glGenBuffers *readbuf;

	writebuf = (struct_glGenBuffers *)wbuf;
	readbuf = (struct_glGenBuffers *)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = func_ID(glGenBuffers);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->n = n;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf->hd.funcID = func_ID(glGenBuffers);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	if(buffers != NULL)
		memcpy(buffers, readbuf->buffers, sizeof(GLuint)*n);
}

void glGenerateMipmap (GLenum target)
{
	int ret;
	struct_glGenerateMipmap *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glGenerateMipmap *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGenerateMipmap);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glGenerateMipmap);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glGenFramebuffers (GLsizei n, GLuint* framebuffers)
{
	int ret;
	struct_glGenFramebuffers *writebuf;
	struct_glGenFramebuffers *readbuf;

	writebuf = (struct_glGenFramebuffers *)wbuf;
	readbuf = (struct_glGenFramebuffers *)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = func_ID(glGenFramebuffers);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->n = n;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf->hd.funcID = func_ID(glGenFramebuffers);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	if(framebuffers != NULL)
		memcpy(framebuffers, readbuf->framebuffers, sizeof(GLuint)*n);
}

void glGenRenderbuffers (GLsizei n, GLuint* renderbuffers)
{
	int ret;
	struct_glGenRenderbuffers *writebuf;
	struct_glGenRenderbuffers *readbuf;

	writebuf = (struct_glGenRenderbuffers *)wbuf;
	readbuf = (struct_glGenRenderbuffers *)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = func_ID(glGenRenderbuffers);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->n = n;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf->hd.funcID = func_ID(glGenRenderbuffers);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	if(renderbuffers != NULL)
		memcpy(renderbuffers, readbuf->renderbuffers, sizeof(GLuint)*n);
}

void glGenTextures (GLsizei n, GLuint* textures)
{
	int ret;
	struct_glGenTextures *writebuf;
	struct_glGenTextures *readbuf;

	writebuf = (struct_glGenTextures *)wbuf;
	readbuf = (struct_glGenTextures *)rbuf;
	init_header(&(writebuf->hd));
	init_header(&(readbuf->hd));

	writebuf->hd.funcID = func_ID(glGenTextures);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->n = n;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf->hd.funcID = func_ID(glGenTextures);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);
	if(textures != NULL)
		memcpy(textures, readbuf->textures, sizeof(GLuint)*n);
}

void glGetActiveAttrib (GLuint program, GLuint index, 
					GLsizei bufsize, GLsizei* length, GLint* size, 
					GLenum* type, GLchar* name)
{
	int ret;
	GLvoid *rbuf = NULL;
	struct_glGetActiveAttrib *writebuf;
	struct_glGetActiveAttrib *readbuf;
	GLsizei struct_length = sizeof(struct_glGetActiveAttrib);
	GLsizei total_length = struct_length + bufsize;
	GLsizei ldata_length = total_length - COMM_HEAD_SIZE;
	
	writebuf = (struct_glGetActiveAttrib *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetActiveAttrib);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->program = program;
	writebuf->index = index;
	writebuf->bufsize = bufsize;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	rbuf = (void *)malloc(total_length);
	memset(rbuf, 0, total_length);
	
	readbuf = (struct_glGetActiveAttrib*)rbuf;
	readbuf->hd.funcID = func_ID(glGetActiveAttrib);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	if(readbuf->hd.ldata_length > 0) {
		ret = read_without_overflow(dev, (rbuf + COMM_HEAD_SIZE), (ldata_length + MAX_BUF_SIZE));
		*size = readbuf->size;
		*type = readbuf->type;
		if(length != NULL)
			*length = readbuf->length;
		if(name == NULL) {
			name = (GLchar*)malloc(bufsize);
			memset(name, 0, bufsize);
		}

		// include null terminator
		memcpy(name, (rbuf + readbuf->name_buf_offset), (readbuf->length + 1));
	}
	else {
		*size = 0;
		*type = 0;
		if(length != NULL) *length = 0;
		name = NULL;
	}
	
	free(rbuf);
}

void glGetActiveUniform (GLuint program, GLuint index, 
					GLsizei bufsize, GLsizei* length, GLint* size, 
					GLenum* type, GLchar* name)
{
	int ret;
	GLvoid *rbuf = NULL;
	struct_glGetActiveUniform *writebuf;
	struct_glGetActiveUniform *readbuf;
	GLsizei struct_length = sizeof(struct_glGetActiveUniform);
	GLsizei total_length = struct_length + bufsize;
	GLsizei ldata_length = total_length - COMM_HEAD_SIZE;
	
	writebuf = (struct_glGetActiveUniform *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetActiveUniform);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->program = program;
	writebuf->index = index;
	writebuf->bufsize = bufsize;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	rbuf = (void *)malloc(total_length);
	memset(rbuf, 0, total_length);
	
	readbuf = (struct_glGetActiveUniform*)rbuf;
	readbuf->hd.funcID = func_ID(glGetActiveUniform);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	if(readbuf->hd.ldata_length > 0) {
		ret = read_without_overflow(dev, (rbuf + COMM_HEAD_SIZE), (ldata_length + MAX_BUF_SIZE));
		*size = readbuf->size;
		*type = readbuf->type;
		if(length != NULL)
			*length = readbuf->length;
		if(name == NULL) {
			name = (GLchar*)malloc(bufsize);
			memset(name, 0, bufsize);
		}

		// include null terminator
		memcpy(name, (rbuf + readbuf->name_buf_offset), (readbuf->length + 1));
	}
	else {
		*size = 0;
		*type = 0;
		if(length != NULL) *length = 0;
		name = NULL;
	}

	free(rbuf);
}

void glGetAttachedShaders (GLuint program, GLsizei maxcount, 
						GLsizei* count, GLuint* shaders)
{
	int ret;
	GLvoid *rbuf = NULL;
	struct_glGetAttachedShaders *writebuf;
	struct_glGetAttachedShaders *readbuf;

	GLsizei struct_length = sizeof(struct_glGetAttachedShaders);
	GLsizei maxcount_byte = maxcount * sizeof(GLuint);
	GLsizei total_length = struct_length + maxcount_byte;
	GLsizei ldata_length = total_length - COMM_HEAD_SIZE;

	writebuf = (struct_glGetAttachedShaders *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetAttachedShaders);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->program = program;
	writebuf->maxcount = maxcount;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	rbuf = (void *)malloc(total_length);
	memset(rbuf, 0, total_length);
	
	readbuf = (struct_glGetAttachedShaders*)rbuf;
	readbuf->hd.funcID = func_ID(glGetAttachedShaders);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	if(readbuf->hd.ldata_length > 0) {
		ret = read_without_overflow(dev, (rbuf + COMM_HEAD_SIZE), (ldata_length + MAX_BUF_SIZE));
		*count = readbuf->count;
		if(shaders == NULL) {
			shaders = (GLuint*)malloc(maxcount_byte);
			memset(shaders, 0, maxcount_byte);
		}

		memcpy(shaders, (rbuf + readbuf->shaders_buf_offset), (readbuf->count * sizeof(GLuint)));
	}
	else {
		*count = 0;
		shaders = NULL;
	}

	free(rbuf);
}

int glGetAttribLocation (GLuint program, const GLchar* name)
{
	int ret;
	int index;
	GLvoid *buf = NULL;
	struct_glGetAttribLocation *writebuf;
	struct_glGetAttribLocation *readbuf;

	GLsizei struct_length = sizeof(struct_glGetAttribLocation);
	GLsizei name_length = strlen(name);
	GLsizei total_length = struct_length + name_length + 1;
	
	buf = (void *)malloc(total_length);
	memset(buf, 0, total_length);

	writebuf = (struct_glGetAttribLocation *)buf;

	writebuf->hd.funcID = func_ID(glGetAttribLocation);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->hd.ldata_length = total_length - COMM_HEAD_SIZE;
	writebuf->program = program;
	writebuf->name_buf_offset = struct_length;

	// include null terminator
	memcpy((buf + writebuf->name_buf_offset), name, (name_length + 1));

	ret = write(dev, writebuf, (total_length + MAX_BUF_SIZE));
	call_log(ret);
	
	readbuf = (struct_glGetAttribLocation*)rbuf;
	readbuf->hd.funcID = func_ID(glGetAttribLocation);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	index = readbuf->retVal;

	free(buf);

	return index;
}

void glGetBooleanv (GLenum pname, GLboolean* params)
{
	int ret, i;
	struct_glGetBooleanv *writebuf;
	struct_glGetBooleanv *readbuf;
	GLuint value_count = 0;

	if(params == NULL) {
		return ;
	}
	
	writebuf = (struct_glGetBooleanv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetBooleanv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->pname = pname;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	value_count = identify_pname_type(pname);

	readbuf = (struct_glGetBooleanv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetBooleanv);
	memset(readbuf->params, 0, sizeof(GLboolean)*16);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	for(i=0; i<value_count; i++) {
		params[i] = readbuf->params[i];
	}
}

void glGetBufferParameteriv (GLenum target, GLenum pname, GLint* params)
{
	int ret;
	struct_glGetBufferParameteriv *writebuf;
	struct_glGetBufferParameteriv *readbuf;

	writebuf = (struct_glGetBufferParameteriv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetBufferParameteriv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	// pname = GL_BUFFER_SIZE or GL_BUFFER_USAGE
	writebuf->pname = pname;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetBufferParameteriv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetBufferParameteriv);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	*params = readbuf->params;
}

GLenum glGetError (void)
{
	int ret;
	GLenum err_value;
	struct_glGetError *writebuf;
	struct_glGetError *readbuf;

	writebuf = (struct_glGetError *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetError);
	writebuf->hd.retFlag = RT_REQUEST;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetError*)rbuf;
	readbuf->hd.funcID = func_ID(glGetError);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	err_value = readbuf->retVal;

	return err_value;
}

void glGetFloatv (GLenum pname, GLfloat* params)
{
	int ret, i;
	struct_glGetFloatv *writebuf;
	struct_glGetFloatv *readbuf;
	GLuint value_count = 0;

	if(params == NULL) {
		return ;
	}
	
	writebuf = (struct_glGetFloatv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetFloatv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->pname = pname;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	value_count = identify_pname_type(pname);

	readbuf = (struct_glGetFloatv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetFloatv);
	memset(readbuf->params, 0, sizeof(GLfloat)*16);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	for(i=0; i<value_count; i++) {
		params[i] = readbuf->params[i];
	}
}

void glGetFramebufferAttachmentParameteriv 
								(GLenum target, GLenum attachment, 
								GLenum pname, GLint* params)
{
	int ret;
	struct_glGetFramebufferAttachmentParameteriv *writebuf;
	struct_glGetFramebufferAttachmentParameteriv *readbuf;

	writebuf = (struct_glGetFramebufferAttachmentParameteriv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetFramebufferAttachmentParameteriv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->attachment = attachment;
	writebuf->pname = pname;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetFramebufferAttachmentParameteriv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetFramebufferAttachmentParameteriv);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	*params = readbuf->params;
}

void glGetIntegerv (GLenum pname, GLint* params)
{
	int ret, i;
	struct_glGetIntegerv *writebuf;
	struct_glGetIntegerv *readbuf;
	GLuint value_count = 0;

	if(params == NULL) {
		return ;
	}
	
	writebuf = (struct_glGetIntegerv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetIntegerv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->pname = pname;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	value_count = identify_pname_type(pname);

	readbuf = (struct_glGetIntegerv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetIntegerv);
	memset(readbuf->params, 0, sizeof(GLint)*16);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	for(i=0; i<value_count; i++) {
		params[i] = readbuf->params[i];
	}
}

void glGetRenderbufferParameteriv (GLenum target, GLenum pname, GLint* params)
{
	int ret;
	struct_glGetRenderbufferParameteriv *writebuf;
	struct_glGetRenderbufferParameteriv *readbuf;

	writebuf = (struct_glGetRenderbufferParameteriv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetRenderbufferParameteriv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->pname = pname;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetRenderbufferParameteriv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetRenderbufferParameteriv);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	*params = readbuf->params;
}

void glGetShaderPrecisionFormat (GLenum shadertype, GLenum precisiontype, 
									GLint* range, GLint* precision)
{
	int ret;
	struct_glGetShaderPrecisionFormat *writebuf;
	struct_glGetShaderPrecisionFormat *readbuf;

	writebuf = (struct_glGetShaderPrecisionFormat *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetShaderPrecisionFormat);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->shadertype = shadertype;
	writebuf->precisiontype = precisiontype;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetShaderPrecisionFormat*)rbuf;
	readbuf->hd.funcID = func_ID(glGetShaderPrecisionFormat);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	range[0] = readbuf->range[0];
	range[1] = readbuf->range[1];
	*precision = readbuf->precision;
}

void glGetShaderSource (GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source)
{
	int ret;
	void *buf = NULL;
	struct_glGetShaderSource *writebuf;
	struct_glGetShaderSource *readbuf;
	
	unsigned int struct_length = 0;
	unsigned int total_length = 0;
	unsigned int ldata_length = 0;

	writebuf = (struct_glGetShaderSource *)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glGetShaderSource);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->shader = shader;
	writebuf->bufsize = bufsize;

	ret= write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	struct_length = sizeof(struct_glGetShaderSource);
	total_length = struct_length + bufsize;
	ldata_length = total_length - COMM_HEAD_SIZE;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	readbuf = (struct_glGetShaderSource *)buf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(glGetShaderSource);

	ret = read_from_shmem(dev, readbuf, COMM_HEAD_SIZE);

	if(readbuf->hd.ldata_length > 0) {
		ret = read_without_overflow(dev, (buf + COMM_HEAD_SIZE), (ldata_length + MAX_BUF_SIZE));
		*length = readbuf->length;
		if(source == NULL) {
			source = (GLchar *)malloc(bufsize);
			memset(source, 0, bufsize);
		}
		// include null terminator
		memcpy(source, (buf + readbuf->source_buf_offset), readbuf->length + 1);
	}
	else {
		*length = 0;
	}

	free(buf);
}

const GLubyte* glGetString (GLenum name)
{
	int ret;
	void *buf = NULL;
	GLubyte *ret_string;
	struct_glGetString *writebuf;
	struct_glGetString *readbuf;

	unsigned int bufsize = MAX_BUF_SIZE;
	unsigned int ret_string_length = 0;
	unsigned int struct_length = 0;
	unsigned int total_length = 0;
	unsigned int ldata_length = 0;

	writebuf = (struct_glGetString *)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glGetString);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->name = name;

	ret= write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	struct_length = sizeof(struct_glGetString);
	total_length = struct_length + bufsize;
	ldata_length = total_length - COMM_HEAD_SIZE;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	readbuf = (struct_glGetString *)buf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(glGetString);

	ret = read_from_shmem(dev, readbuf, COMM_HEAD_SIZE);

	if(readbuf->hd.ldata_length > 0) {
		ret = read_without_overflow(dev, (buf + COMM_HEAD_SIZE), (ldata_length + MAX_BUF_SIZE));
		ret_string_length = strlen(buf + readbuf->retVal_buf_offset);

		// include null terminator
		ret_string = (GLubyte *)malloc(ret_string_length + 1);
		memset(ret_string, 0, ret_string_length + 1);
		memcpy(ret_string, (buf + readbuf->retVal_buf_offset), ret_string_length + 1);
	}
	else {
		ret_string = NULL;
	}

	free(buf);

	add_to_unfreed_list(unfreed, unfreed_tail, ret_string);

	return ret_string;
}

void glGetTexParameterfv (GLenum target, GLenum pname, GLfloat* params)
{
	int ret;
	struct_glGetTexParameterfv *writebuf;
	struct_glGetTexParameterfv *readbuf;

	writebuf = (struct_glGetTexParameterfv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetTexParameterfv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->pname = pname;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetTexParameterfv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetTexParameterfv);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	*params = readbuf->params;
}

void glGetTexParameteriv (GLenum target, GLenum pname, GLint* params)
{
	int ret;
	struct_glGetTexParameteriv *writebuf;
	struct_glGetTexParameteriv *readbuf;

	writebuf = (struct_glGetTexParameteriv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetTexParameteriv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->pname = pname;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetTexParameteriv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetTexParameteriv);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	*params = readbuf->params;
}

// if location is in a shader, params is one value
// if values is defined as a vec2 type, params is two values
// if values is defined as a vec3 type, params is three values
// and so on
void glGetUniformfv (GLuint program, GLint location, GLfloat* params)
{
	int ret, i;
	struct_glGetUniformfv *writebuf;
	struct_glGetUniformfv *readbuf;
	GLuint count = 0;

	writebuf = (struct_glGetUniformfv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetUniformfv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->program = program;
	writebuf->location = location;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetUniformfv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetUniformfv);
	memset(&readbuf->params, 0, sizeof(GLfloat)*16);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	count = readbuf->count;
	for(i=0; i<readbuf->count; i++)
		params[i] = readbuf->params[i];
}

void glGetUniformiv (GLuint program, GLint location, GLint* params)
{
	int ret, i;
	struct_glGetUniformiv *writebuf;
	struct_glGetUniformiv *readbuf;
	GLuint count = 0;

	writebuf = (struct_glGetUniformiv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetUniformiv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->program = program;
	writebuf->location = location;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetUniformiv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetUniformiv);
	memset(&readbuf->params, 0, sizeof(GLint)*16);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	count = readbuf->count;
	for(i=0; i<readbuf->count; i++)
		params[i] = readbuf->params[i];
}

void glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat* params)
{
	int ret, i;
	struct_glGetVertexAttribfv *writebuf;
	struct_glGetVertexAttribfv *readbuf;

	writebuf = (struct_glGetVertexAttribfv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetVertexAttribfv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->index = index;
	writebuf->pname = pname;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetVertexAttribfv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetVertexAttribfv);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	if(pname == GL_CURRENT_VERTEX_ATTRIB) {
		for(i=0; i<4; i++)
			params[i] = readbuf->params[i];
	}
	else
		*params = readbuf->params[0];
}

void glGetVertexAttribiv (GLuint index, GLenum pname, GLint* params)
{
	int ret, i;
	struct_glGetVertexAttribiv *writebuf;
	struct_glGetVertexAttribiv *readbuf;

	writebuf = (struct_glGetVertexAttribiv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetVertexAttribiv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->index = index;
	writebuf->pname = pname;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetVertexAttribiv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetVertexAttribiv);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	if(pname == GL_CURRENT_VERTEX_ATTRIB) {
		for(i=0; i<4; i++)
			params[i] = readbuf->params[i];
	}
	else
		*params = readbuf->params[0];
}

void glGetVertexAttribPointerv (GLuint index, GLenum pname, GLvoid** pointer)
{
	int ret;
	struct_glGetVertexAttribPointerv *writebuf;
	struct_glGetVertexAttribPointerv *readbuf;

	writebuf = (struct_glGetVertexAttribPointerv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glGetVertexAttribPointerv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->index = index;
	writebuf->pname = pname;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glGetVertexAttribPointerv*)rbuf;
	readbuf->hd.funcID = func_ID(glGetVertexAttribPointerv);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	*pointer = readbuf->pointer;
}

void glHint (GLenum target, GLenum mode)
{
	int ret;
	struct_glHint *writebuf;

	writebuf = (struct_glHint *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glHint);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->target = target;
	writebuf->mode = mode;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

// this function requires the return value
GLboolean glIsBuffer (GLuint buffer)
{
	int ret;
	struct_glIsBuffer *writebuf;
	struct_glIsBuffer *readbuf;
	GLboolean result;

	writebuf = (struct_glIsBuffer *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glIsBuffer);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->buffer = buffer;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glIsBuffer*)rbuf;
	init_header(&(readbuf->hd));
	
	readbuf->hd.funcID = func_ID(glIsBuffer);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

// this function requires the return value
GLboolean glIsEnabled (GLenum cap)
{
	int ret;
	struct_glIsEnabled *writebuf;
	struct_glIsEnabled *readbuf;
	GLboolean result;

	writebuf = (struct_glIsEnabled *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glIsEnabled);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->cap = cap;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glIsEnabled*)rbuf;
	init_header(&(readbuf->hd));
	
	readbuf->hd.funcID = func_ID(glIsEnabled);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

// this function requires the return value
GLboolean glIsFramebuffer (GLuint framebuffer)
{
	int ret;
	struct_glIsFramebuffer *writebuf;
	struct_glIsFramebuffer *readbuf;
	GLboolean result;

	writebuf = (struct_glIsFramebuffer *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glIsFramebuffer);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->framebuffer = framebuffer;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glIsFramebuffer*)rbuf;
	init_header(&(readbuf->hd));
	
	readbuf->hd.funcID = func_ID(glIsFramebuffer);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

// this function requires the return value
GLboolean glIsProgram (GLuint program)
{
	int ret;
	struct_glIsProgram *writebuf;
	struct_glIsProgram *readbuf;
	GLboolean result;

	writebuf = (struct_glIsProgram *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glIsProgram);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->program = program;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glIsProgram*)rbuf;
	init_header(&(readbuf->hd));
	
	readbuf->hd.funcID = func_ID(glIsProgram);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

// this function requires the return value
GLboolean glIsRenderbuffer (GLuint renderbuffer)
{
	int ret;
	struct_glIsRenderbuffer *writebuf;
	struct_glIsRenderbuffer *readbuf;
	GLboolean result;

	writebuf = (struct_glIsRenderbuffer *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glIsRenderbuffer);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->renderbuffer = renderbuffer;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glIsRenderbuffer*)rbuf;
	init_header(&(readbuf->hd));
	
	readbuf->hd.funcID = func_ID(glIsRenderbuffer);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

// this function requires the return value
GLboolean glIsShader (GLuint shader)
{
	int ret;
	struct_glIsShader *writebuf;
	struct_glIsShader *readbuf;
	GLboolean result;

	writebuf = (struct_glIsShader *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glIsShader);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->shader = shader;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glIsShader*)rbuf;
	init_header(&(readbuf->hd));
	
	readbuf->hd.funcID = func_ID(glIsShader);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

// this function requires the return value
GLboolean glIsTexture (GLuint texture)
{
	int ret;
	struct_glIsTexture *writebuf;
	struct_glIsTexture *readbuf;
	GLboolean result;

	writebuf = (struct_glIsTexture *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glIsTexture);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->texture = texture;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	readbuf = (struct_glIsTexture*)rbuf;
	init_header(&(readbuf->hd));
	
	readbuf->hd.funcID = func_ID(glIsTexture);

	ret = read_from_shmem(dev, readbuf, MAX_BUF_SIZE);

	result = readbuf->retVal;

	return result;
}

void glLineWidth (GLfloat width)
{
	int ret;
	struct_glLineWidth *writebuf;

	writebuf = (struct_glLineWidth *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glLineWidth);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->width = width;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glPixelStorei (GLenum pname, GLint param)
{
	int ret;
	struct_glPixelStorei *writebuf;

	writebuf = (struct_glPixelStorei *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glPixelStorei);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->pname = pname;
	writebuf->param = param;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glPolygonOffset (GLfloat factor, GLfloat units)
{
	int ret;
	struct_glPolygonOffset *writebuf;

	writebuf = (struct_glPolygonOffset *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glPolygonOffset);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->factor = factor;
	writebuf->units = units;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, 
				GLenum format, GLenum type, GLvoid* pixels)
{
	int ret;
	void *buf;
	struct_glReadPixels *writebuf;
	struct_glReadPixels *readbuf;

//	GLsizei bytes_per_pixel = 0;
	GLsizei struct_length = sizeof(struct_glReadPixels);
	GLsizei total_length = 0;
	GLsizei ldata_length = 0;
	GLsizei pixel_buf_length = 0;

	writebuf = (struct_glReadPixels *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glReadPixels);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->x = x;
	writebuf->y = y;
	writebuf->width = width;
	writebuf->height = height;
	writebuf->format = format;
	writebuf->type = type;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

//	bytes_per_pixel = get_bytes_per_pixel(format, type);
//	pixel_buf_length = (width * height) * bytes_per_pixel;

	pixel_buf_length = get_bytes_per_image(width, height, format, type);

	total_length = struct_length + pixel_buf_length;
	ldata_length = total_length - COMM_HEAD_SIZE;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	readbuf = (struct_glReadPixels *)buf;
	init_header(&(readbuf->hd));

	readbuf->hd.funcID = func_ID(glReadPixels);

	ret = read_from_shmem(dev, readbuf, COMM_HEAD_SIZE);

	if(readbuf->hd.ldata_length > 0) {
		ret = read_without_overflow(dev, (buf + COMM_HEAD_SIZE), (ldata_length + MAX_BUF_SIZE));

		if(pixels == NULL) {
			pixels = (GLvoid *)malloc(pixel_buf_length);
			memset(pixels, 0, pixel_buf_length);
		}
		memcpy(pixels, (buf + readbuf->pixels_buf_offset), pixel_buf_length);
	}

	free(buf);
}

void glReleaseShaderCompiler (void)
{
	int ret;
	struct_glReleaseShaderCompiler *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glReleaseShaderCompiler *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glReleaseShaderCompiler);
	writebuf->hd.retFlag = RT_REQUEST;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glReleaseShaderCompiler);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glRenderbufferStorage (GLenum target, GLenum internalformat, 
							GLsizei width, GLsizei height)
{
	int ret;
	struct_glRenderbufferStorage *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glRenderbufferStorage *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glRenderbufferStorage);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->internalformat = internalformat;
	writebuf->width = width;
	writebuf->height = height;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glRenderbufferStorage);

	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glSampleCoverage (GLclampf value, GLboolean invert)
{
	int ret;
	struct_glSampleCoverage *writebuf;

	writebuf = (struct_glSampleCoverage *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glSampleCoverage);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->value = value;
	writebuf->invert = invert;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glScissor (GLint x, GLint y, GLsizei width, GLsizei height)
{
	int ret;
	struct_glScissor *writebuf;

	writebuf = (struct_glScissor *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glScissor);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->x = x;
	writebuf->y = y;
	writebuf->width = width;
	writebuf->height = height;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glShaderBinary (GLsizei n, const GLuint* shaders, 
					GLenum binaryformat, const GLvoid* binary, GLsizei length)
{
	int ret;
	void *buf = NULL;
	struct_glShaderBinary *writebuf;
	typ_common_hd readhd;
	
	GLsizei struct_length = sizeof(struct_glShaderBinary);
	GLsizei shaders_length = sizeof(GLuint) * n;
	GLsizei total_length = 0;

	total_length = struct_length + shaders_length + length;

	buf = (void*)malloc(total_length);
	memset(buf, 0, total_length);

	writebuf = (struct_glShaderBinary *)buf;

	writebuf->hd.funcID = func_ID(glShaderBinary);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->n = n;
	writebuf->shaders_buf_offset = struct_length;
	writebuf->binaryformat = binaryformat;
	writebuf->binary_buf_offset = writebuf->shaders_buf_offset + shaders_length;
	writebuf->length = length;

	writebuf->hd.ldata_length = total_length - COMM_HEAD_SIZE;
	if(shaders != NULL)
		memcpy((buf + writebuf->shaders_buf_offset), shaders, shaders_length);
	if(binary != NULL)
		memcpy((buf + writebuf->binary_buf_offset), binary, length);

	// Because write size is total_length + MAX_BUF_SIZE, 
	// the write size is always bigger than MAX_BUF_SIZE.
	// Write function in kernel copies the large data with "write size - MAX_BUF_SIZE"
	ret = write(dev, buf, (total_length + MAX_BUF_SIZE));
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glShaderBinary);

	// Wait to empty the ldata_buf, until secure world read the data
	Ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);

	free(buf);
}

void glStencilFunc (GLenum func, GLint ref, GLuint mask)
{
	int ret;
	struct_glStencilFunc *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glStencilFunc *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glStencilFunc);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->func = func;
	writebuf->ref = ref;
	writebuf->mask = mask;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glStencilFunc);

	// Wait to empty the ldata_buf, until secure world read the data
	Ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glStencilFuncSeparate (GLenum face, GLenum func, GLint ref, GLuint mask)
{
	int ret;
	struct_glStencilFuncSeparate *writebuf;

	writebuf = (struct_glStencilFuncSeparate *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glStencilFuncSeparate);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->face = face;
	writebuf->func = func;
	writebuf->ref = ref;
	writebuf->mask = mask;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glStencilMask (GLuint mask)
{
	int ret;
	struct_glStencilMask *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glStencilMask *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glStencilMask);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->mask = mask;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glStencilMask);

	// Wait to empty the ldata_buf, until secure world read the data
	Ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glStencilMaskSeparate (GLenum face, GLuint mask)
{
	int ret;
	struct_glStencilMaskSeparate *writebuf;

	writebuf = (struct_glStencilMaskSeparate *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glStencilMaskSeparate);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->face = face;
	writebuf->mask = mask;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass)
{
	int ret;
	struct_glStencilOp *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glStencilOp *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glStencilOp);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->fail = fail;
	writebuf->zfail = zfail;
	writebuf->zpass = zpass;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glStencilOp);

	// Wait to empty the ldata_buf, until secure world read the data
	Ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glStencilOpSeparate (GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
	int ret;
	struct_glStencilOpSeparate *writebuf;

	writebuf = (struct_glStencilOpSeparate *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glStencilOpSeparate);
	writebuf->hd.retFlag = RT_NONE;
	writebuf->face = face;
	writebuf->fail = fail;
	writebuf->zfail = zfail;
	writebuf->zpass = zpass;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glTexImage2D (GLenum target, GLint level, GLint internalformat, 
				GLsizei width, GLsizei height, GLint border, GLenum format, 
				GLenum type, const GLvoid* pixels)
{
	int ret, i;
	void *buf = NULL;
	const GLvoid *curr_pixels = NULL;
	struct_glTexImage2D *writebuf;
	typ_common_hd readhd;
	
	GLsizei struct_length = sizeof(struct_glTexImage2D);
	GLsizei pixel_data_length = 0;

	GLsizei one_ldata_length = 0;
	GLsizei last_ldata_length = 0;
	GLsizei ldata_count = 0;
/*
	int row = 0, start_of_row = 0, start_of_rev_row = 0;
	GLsizei bytes_per_pixel = get_bytes_per_pixel(format, type);
	GLsizei bytes_per_row = width * bytes_per_pixel;
	GLvoid *reverse_pixels = NULL;
*/
	pixel_data_length = get_bytes_per_image(width, height, format, type);

	ldata_count = pixel_data_length / MAX_LDATA_SIZE_1M;
	last_ldata_length = pixel_data_length % MAX_LDATA_SIZE_1M;

	writebuf = (struct_glTexImage2D *)wbuf;

	writebuf->hd.funcID = func_ID(glTexImage2D);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->level = level;
	writebuf->internalformat = internalformat;
	writebuf->width = width;
	writebuf->height = height;
	writebuf->border = border;
	writebuf->format = format;
	writebuf->type = type;

	if(pixels != NULL) {
		writebuf->pixels_buf_offset = struct_length;
		writebuf->hd.ldata_length = pixel_data_length;

		// send arguments except "pixels"
		ret = write(dev, writebuf, struct_length);

		// all divided "pixels" data have to include the common header to identify the function id
		one_ldata_length = COMM_HEAD_SIZE + MAX_LDATA_SIZE_1M;
		buf = (void*)malloc(one_ldata_length);
		memset(buf, 0, one_ldata_length);

/*		reverse_pixels = malloc(pixel_data_length);
		memset(reverse_pixels, 0, pixel_data_length);

		for(row=height; row>0; row--) {
			start_of_row = bytes_per_row * (row - 1);
			start_of_rev_row = bytes_per_row * (height - row);
			memcpy((reverse_pixels + start_of_rev_row), (pixels + start_of_row), bytes_per_row);
		}
*/
		ret = ioctl(dev, IMMEDIATE_SEND_START);

//		curr_pixels = reverse_pixels;
		curr_pixels = pixels;
		for(i=0; i<ldata_count; i++) {
			// wait for request for remained data
			readhd.funcID = func_ID(glTexImage2D);
			ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);

			memcpy(buf, writebuf, COMM_HEAD_SIZE);
			memcpy((buf+COMM_HEAD_SIZE), curr_pixels, MAX_LDATA_SIZE_1M);

			ret = write(dev, buf, (one_ldata_length + MAX_BUF_SIZE));

			curr_pixels = curr_pixels + MAX_LDATA_SIZE_1M;
		}

		if(last_ldata_length > 0) {
			// wait for request for remained data
			readhd.funcID = func_ID(glTexImage2D);
			ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);

			memcpy(buf, writebuf, COMM_HEAD_SIZE);
			memcpy((buf+COMM_HEAD_SIZE), curr_pixels, last_ldata_length);

			ret = write(dev, buf, (last_ldata_length + COMM_HEAD_SIZE + MAX_BUF_SIZE));
		}
		ret = ioctl(dev, IMMEDIATE_SEND_STOP);
	}
	else {
		writebuf->pixels_buf_offset = 0;
		writebuf->hd.ldata_length = 0;

		ret = write(dev, writebuf, struct_length);
	}
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glTexImage2D);

	// Wait to empty the ldata_buf, until secure world read the data
	Ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);

//	if(reverse_pixels != NULL)
//		free(reverse_pixels);

	if(buf != NULL)
		free(buf);
}

void glTexParameterf (GLenum target, GLenum pname, GLfloat param)
{
	int ret;
	struct_glTexParameterf *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glTexParameterf *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glTexParameterf);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->pname = pname;
	writebuf->param = param;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glTexParameterf);

	// Wait to empty the ldata_buf, until secure world read the data
	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glTexParameterfv (GLenum target, GLenum pname, const GLfloat* params)
{
	int ret;
	struct_glTexParameterfv *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glTexParameterfv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glTexParameterfv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->pname = pname;

	// params is an array with four values as RGBA
	// gles 2.0 is not support GL_TEXTURE_BORDER_COLOR
//	if(pname == GL_TEXTURE_BORDER_COLOR)
//		memcpy(writebuf->params, params, sizeof(GLfloat) * 4);
//	else
		writebuf->params[0] = *params;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glTexParameterfv);

	// Wait to empty the ldata_buf, until secure world read the data
	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glTexParameteri (GLenum target, GLenum pname, GLint param)
{
	int ret;
	struct_glTexParameteri *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glTexParameteri *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glTexParameteri);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->pname = pname;
	writebuf->param = param;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glTexParameteri);

	// Wait to empty the ldata_buf, until secure world read the data
	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glTexParameteriv (GLenum target, GLenum pname, const GLint* params)
{
	int ret;
	struct_glTexParameteriv *writebuf;
	typ_common_hd readhd;

	writebuf = (struct_glTexParameteriv *)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glTexParameteriv);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->pname = pname;

	// params is an array with four values as RGBA
	// gles 2.0 is not support GL_TEXTURE_BORDER_COLOR
//	if(pname == GL_TEXTURE_BORDER_COLOR)
//		memcpy(writebuf->params, params, sizeof(GLint) * 4);
//	else
		writebuf->params[0] = *params;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glTexParameteriv);

	// Wait to empty the ldata_buf, until secure world read the data
	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);
}

void glTexSubImage2D (GLenum target, GLint level, 
					GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, 
					GLenum format, GLenum type, const GLvoid* pixels)
{
	int ret, i;
	void *buf = NULL;
	const GLvoid *curr_pixels = NULL;
	struct_glTexSubImage2D *writebuf;
	typ_common_hd readhd;
	
	GLsizei struct_length = sizeof(struct_glTexSubImage2D);
	GLsizei pixel_data_length = 0;

	GLsizei one_ldata_length = 0;
	GLsizei last_ldata_length = 0;
	GLsizei ldata_count = 0;
/*
	int row = 0, start_of_row = 0, start_of_rev_row = 0;
	GLsizei bytes_per_pixel = get_bytes_per_pixel(format, type);
	GLsizei bytes_per_row = width * bytes_per_pixel;
	GLvoid *reverse_pixels = NULL;
*/
	pixel_data_length = get_bytes_per_image(width, height, format, type);

	ldata_count = pixel_data_length / MAX_LDATA_SIZE_1M;
	last_ldata_length = pixel_data_length % MAX_LDATA_SIZE_1M;

	writebuf = (struct_glTexSubImage2D *)wbuf;

	writebuf->hd.funcID = func_ID(glTexSubImage2D);
	writebuf->hd.retFlag = RT_REQUEST;
	writebuf->target = target;
	writebuf->level = level;
	writebuf->xoffset = xoffset;
	writebuf->yoffset = yoffset;
	writebuf->width = width;
	writebuf->height = height;
	writebuf->format = format;
	writebuf->type = type;

	if(pixels != NULL) {
		writebuf->hd.ldata_length = pixel_data_length;
		writebuf->pixels_buf_offset = struct_length;

		// send arguments except "pixels"
		ret = write(dev, writebuf, struct_length);

		// all divided "pixels" data have to include the common header to identify the function id
		one_ldata_length = COMM_HEAD_SIZE + MAX_LDATA_SIZE_1M;
		buf = (void*)malloc(one_ldata_length);
		memset(buf, 0, one_ldata_length);

/*		reverse_pixels = malloc(pixel_data_length);
		memset(reverse_pixels, 0, pixel_data_length);

		for(row=height; row>0; row--) {
			start_of_row = bytes_per_row * (row - 1);
			start_of_rev_row = bytes_per_row * (height - row);
			memcpy((reverse_pixels + start_of_rev_row), (pixels + start_of_row), bytes_per_row);
		}
*/
		ret = ioctl(dev, IMMEDIATE_SEND_START);

//		curr_pixels = reverse_pixels;
		curr_pixels = pixels;
		for(i=0; i<ldata_count; i++) {
			// wait for request for remained data
			readhd.funcID = func_ID(glTexSubImage2D);
			ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);

			memcpy(buf, writebuf, COMM_HEAD_SIZE);
			memcpy((buf+COMM_HEAD_SIZE), curr_pixels, MAX_LDATA_SIZE_1M);

			ret = write(dev, buf, (one_ldata_length + MAX_BUF_SIZE));

			curr_pixels = curr_pixels + MAX_LDATA_SIZE_1M;
		}
		
		if(last_ldata_length > 0) {
			// wait for request for remained data
			readhd.funcID = func_ID(glTexSubImage2D);
			ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);

			memcpy(buf, writebuf, COMM_HEAD_SIZE);
			memcpy((buf+COMM_HEAD_SIZE), curr_pixels, last_ldata_length);

			ret = write(dev, buf, (last_ldata_length + COMM_HEAD_SIZE + MAX_BUF_SIZE));
		}
		ret = ioctl(dev, IMMEDIATE_SEND_STOP);
	}
	else {
		writebuf->hd.ldata_length = 0;
		writebuf->pixels_buf_offset = 0;
		ret = write(dev, writebuf, struct_length);
	}
	call_log(ret);

	init_header(&readhd);
	readhd.funcID = func_ID(glTexSubImage2D);

	// Wait to empty the ldata_buf, until secure world read the data
	ret = read_from_shmem(dev, &readhd, COMM_HEAD_SIZE);

//	if(reverse_pixels != NULL)
//		free(reverse_pixels);
	if(buf != NULL)
		free(buf);
}


////////// 0604 completion ///////////////
void glUniform1f (GLint location, GLfloat x)
{
	int ret;
	struct_glUniform1f *writebuf;

	writebuf = (struct_glUniform1f*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform1f);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->x = x;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform1fv (GLint location, GLsizei count, const GLfloat* v)
{
	int ret;
	struct_glUniform1fv *writebuf;
	unsigned int dim = 1;
	unsigned int vertex_cnt =0;

	writebuf = (struct_glUniform1fv*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform1fv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->count = count;

	vertex_cnt = (dim * count);
	memcpy(writebuf->v, v, sizeof(GLfloat)*vertex_cnt);
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform1i (GLint location, GLint x)
{
	int ret;
	struct_glUniform1i *writebuf;

	writebuf = (struct_glUniform1i*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform1i);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->x = x;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform1iv (GLint location, GLsizei count, const GLint* v)
{
	int ret;
	struct_glUniform1iv *writebuf;
	unsigned int dim = 1;
	unsigned int vertex_cnt =0;

	writebuf = (struct_glUniform1iv*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform1iv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->count = count;

	vertex_cnt = (dim * count);
	memcpy(writebuf->v, v, sizeof(GLint)*vertex_cnt);
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform2f (GLint location, GLfloat x, GLfloat y)
{
	int ret;
	struct_glUniform2f *writebuf;

	writebuf = (struct_glUniform2f*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform2f);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->x = x;
	writebuf->y = y;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform2fv (GLint location, GLsizei count, const GLfloat* v)
{
	int ret;
	struct_glUniform2fv *writebuf;
	unsigned int dim = 2;
	unsigned int vertex_cnt =0;

	writebuf = (struct_glUniform2fv*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform2fv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->count = count;

	vertex_cnt = (dim * count);
	memcpy(writebuf->v, v, sizeof(GLfloat)*vertex_cnt);
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform2i (GLint location, GLint x, GLint y)
{
	int ret;
	struct_glUniform2i *writebuf;

	writebuf = (struct_glUniform2i*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform2i);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->x = x;
	writebuf->y = y;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform2iv (GLint location, GLsizei count, const GLint* v)
{
	int ret;
	struct_glUniform2iv *writebuf;
	unsigned int dim = 2;
	unsigned int vertex_cnt =0;

	writebuf = (struct_glUniform2iv*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform2iv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->count = count;

	vertex_cnt = (dim * count);
	memcpy(writebuf->v, v, sizeof(GLint)*vertex_cnt);
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform3f (GLint location, GLfloat x, GLfloat y, GLfloat z)
{
	int ret;
	struct_glUniform3f *writebuf;

	writebuf = (struct_glUniform3f*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform3f);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->x = x;
	writebuf->y = y;
	writebuf->z = z;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform3fv (GLint location, GLsizei count, const GLfloat* v)
{
	int ret;
	struct_glUniform3fv *writebuf;
	unsigned int dim = 3;
	unsigned int vertex_cnt =0;

	writebuf = (struct_glUniform3fv*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform3fv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->count = count;

	vertex_cnt = (dim * count);
	memcpy(writebuf->v, v, sizeof(GLfloat)*vertex_cnt);
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform3i (GLint location, GLint x, GLint y, GLint z)
{
	int ret;
	struct_glUniform3i *writebuf;

	writebuf = (struct_glUniform3i*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform3i);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->x = x;
	writebuf->y = y;
	writebuf->z = z;

	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform3iv (GLint location, GLsizei count, const GLint* v)
{
	int ret;
	struct_glUniform3iv *writebuf;
	unsigned int dim = 3;
	unsigned int vertex_cnt =0;

	writebuf = (struct_glUniform3iv*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform3iv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->count = count;

	vertex_cnt = (dim * count);
	memcpy(writebuf->v, v, sizeof(GLint)*vertex_cnt);
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform4f (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	int ret;
	struct_glUniform4f *writebuf;

	writebuf = (struct_glUniform4f*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform4f);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->x = x;
	writebuf->y = y;
	writebuf->z = z;
	writebuf->w = w;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform4fv (GLint location, GLsizei count, const GLfloat* v)
{
	int ret;
	struct_glUniform4fv *writebuf;
	unsigned int dim = 4;
	unsigned int vertex_cnt =0;

	writebuf = (struct_glUniform4fv*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform4fv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->count = count;

	vertex_cnt = (dim * count);
	memcpy(writebuf->v, v, sizeof(GLfloat)*vertex_cnt);
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform4i (GLint location, GLint x, GLint y, GLint z, GLint w)
{
	int ret;
	struct_glUniform4i *writebuf;

	writebuf = (struct_glUniform4i*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform4i);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->x = x;
	writebuf->y = y;
	writebuf->z = z;
	writebuf->w = w;
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniform4iv (GLint location, GLsizei count, const GLint* v)
{
	int ret;
	struct_glUniform4iv *writebuf;
	unsigned int dim = 1;
	unsigned int vertex_cnt =0;

	writebuf = (struct_glUniform4iv*)wbuf;
	init_header(&(writebuf->hd));
	
	writebuf->hd.funcID = func_ID(glUniform4iv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->count = count;

	vertex_cnt = (dim * count);
	memcpy(writebuf->v, v, sizeof(GLint)*vertex_cnt);
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	struct_glUniformMatrix2fv *writebuf;

	writebuf = (struct_glUniformMatrix2fv*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glUniformMatrix2fv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->count = count;
	writebuf->transpose = transpose;

	memset(writebuf->value, 0, sizeof(GLfloat)*4);
	if(value != NULL) {
		memcpy(writebuf->value, value, sizeof(GLfloat)*4);
	}
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	struct_glUniformMatrix3fv *writebuf;

	writebuf = (struct_glUniformMatrix3fv*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glUniformMatrix3fv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->location = location;
	writebuf->count = count;
	writebuf->transpose = transpose;

	memset(writebuf->value, 0, sizeof(GLfloat)*9);
	if(value != NULL) {
		memcpy(writebuf->value, value, sizeof(GLfloat)*9);
	}
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glValidateProgram (GLuint program)
{
	struct_glValidateProgram *writebuf;

	writebuf = (struct_glValidateProgram*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glValidateProgram);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->program = program;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glVertexAttrib1f (GLuint indx, GLfloat x)
{
	struct_glVertexAttrib1f *writebuf;

	writebuf = (struct_glVertexAttrib1f*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glVertexAttrib1f);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->indx = indx;
	writebuf->x = x;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glVertexAttrib1fv (GLuint indx, const GLfloat* values)
{
	int ret;
	struct_glVertexAttrib1fv *writebuf;
	unsigned int dim = 1;

	writebuf = (struct_glVertexAttrib1fv*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glVertexAttrib1fv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->indx = indx;

	// Array to be used for the vertex attribute
	// But, it only modifies the first one component of the vertex array.
	// So, values is one component of "values".
	memset(writebuf->values, 0, sizeof(GLfloat)*dim);
	if(values != NULL) {
		memcpy(writebuf->values, values, sizeof(GLfloat)*dim);
	}
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glVertexAttrib2f (GLuint indx, GLfloat x, GLfloat y)
{
	struct_glVertexAttrib2f *writebuf;

	writebuf = (struct_glVertexAttrib2f*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glVertexAttrib2f);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->indx = indx;
	writebuf->x = x;
	writebuf->y = y;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glVertexAttrib2fv (GLuint indx, const GLfloat* values)
{
	int ret;
	struct_glVertexAttrib2fv *writebuf;
	unsigned int dim = 2;

	writebuf = (struct_glVertexAttrib2fv*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glVertexAttrib2fv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->indx = indx;

	// Array to be used for the vertex attribute
	// But, it only modifies the first two components of the vertex array.
	// So, values is two components of "values".
	memset(writebuf->values, 0, sizeof(GLfloat)*dim);
	if(values != NULL) {
		memcpy(writebuf->values, values, sizeof(GLfloat)*dim);
	}
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glVertexAttrib3f (GLuint indx, GLfloat x, GLfloat y, GLfloat z)
{
	struct_glVertexAttrib3f *writebuf;

	writebuf = (struct_glVertexAttrib3f*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glVertexAttrib3f);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->indx = indx;
	writebuf->x = x;
	writebuf->y = y;
	writebuf->z = z;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glVertexAttrib3fv (GLuint indx, const GLfloat* values)
{
	int ret;
	struct_glVertexAttrib3fv *writebuf;
	unsigned int dim = 3;

	writebuf = (struct_glVertexAttrib3fv*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glVertexAttrib3fv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->indx = indx;

	// Array to be used for the vertex attribute
	// But, it only modifies the first three components of the vertex array.
	// So, values is three components of "values".
	memset(writebuf->values, 0, sizeof(GLfloat)*dim);
	if(values != NULL) {
		memcpy(writebuf->values, values, sizeof(GLfloat)*dim);
	}
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}

void glVertexAttrib4f (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	struct_glVertexAttrib4f *writebuf;

	writebuf = (struct_glVertexAttrib4f*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glVertexAttrib4f);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->indx = indx;
	writebuf->x = x;
	writebuf->y = y;
	writebuf->z = z;
	writebuf->w = w;
	
	Ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(Ret);
}

void glVertexAttrib4fv (GLuint indx, const GLfloat* values)
{
	int ret;
	struct_glVertexAttrib4fv *writebuf;
	unsigned int dim = 4;

	writebuf = (struct_glVertexAttrib4fv*)wbuf;
	init_header(&(writebuf->hd));

	writebuf->hd.funcID = func_ID(glVertexAttrib4fv);
	writebuf->hd.retFlag = RT_NONE;

	writebuf->indx = indx;

	// Array to be used for the vertex attribute
	// But, it only modifies the first four components of the vertex array.
	// So, values is four components of "values".
	memset(writebuf->values, 0, sizeof(GLfloat)*dim);
	if(values != NULL) {
		memcpy(writebuf->values, values, sizeof(GLfloat)*dim);
	}
	
	ret = write(dev, writebuf, MAX_BUF_SIZE);
	call_log(ret);
}


