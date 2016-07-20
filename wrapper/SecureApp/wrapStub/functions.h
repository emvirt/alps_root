/* GLESv2 functions */
int call_glEnable(int* pDatabuf);
int call_glClear(int* pDatabuf);
int call_glClearColor(int* pDatabuf);
int call_glDisableVertexAttribArray(int *pDatabuf);
int call_glUniformMatrix4fv(int *pDatabuf);
int call_glEnableVertexAttribArray(int *pDatabuf);
int call_glDrawArrays(int *pDatabuf);
int call_glVertexAttribPointer(int *pDatabuf);
int call_glViewport(int *pDatabuf);
int call_glCreateShader(int *pDatabuf);				//10

int call_glShaderSource(int *pDatabuf);
int call_glCompileShader(int *pDatabuf);
int call_glGetShaderiv(int *pDatabuf);
int call_glGetShaderInfoLog(int *pDatabuf);
int call_glCreateProgram(int *pDatabuf);
int call_glAttachShader(int *pDatabuf);
int call_glBindAttribLocation(int *pDatabuf);
int call_glLinkProgram(int *pDatabuf);
int call_glGetProgramiv(int *pDatabuf);
int call_glGetProgramInfoLog(int *pDatabuf);		//20

int call_glGetUniformLocation(int *pDatabuf);
int call_glDeleteShader(int *pDatabuf);
int call_glUseProgram(int *pDatabuf);
int call_glActiveTexture(int *pDatabuf);
int call_glBindBuffer(int *pDatabuf);
int call_glBindFramebuffer(int *pDatabuf);
int call_glBindRenderbuffer(int *pDatabuf);
int call_glBindTexture(int *pDatabuf);
int call_glBlendColor(int *pDatabuf);
int call_glBlendEquation(int *pDatabuf);			//30

int call_glBlendEquationSeparate(int *pDatabuf);
int call_glBlendFunc(int *pDatabuf);
int call_glBlendFuncSeparate(int *pDatabuf);
int call_glBufferData(int *pDatabuf);
int call_glBufferSubData(int *pDatabuf);
int call_glCheckFramebufferStatus(int *pDatabuf);
int call_glClearDepthf(int *pDatabuf);
int call_glClearStencil(int *pDatabuf);
int call_glColorMask(int *pDatabuf);
int call_glCompressedTexImage2D(int *pDatabuf);	//40

int call_glCompressedTexSubImage2D(int *pDatabuf);
int call_glCopyTexImage2D(int *pDatabuf);
int call_glCopyTexSubImage2D(int *pDatabuf);
int call_glCullFace(int *pDatabuf);
int call_glDeleteBuffers(int *pDatabuf);
int call_glDeleteFramebuffers(int *pDatabuf);
int call_glDeleteProgram(int *pDatabuf);
int call_glDeleteRenderbuffers(int *pDatabuf);
int call_glDeleteTextures(int *pDatabuf);
int call_glDepthFunc(int *pDatabuf);				//50

int call_glDepthMask(int *pDatabuf);
int call_glDepthRangef(int *pDatabuf);
int call_glDetachShader(int *pDatabuf);
int call_glDisable(int *pDatabuf);
int call_glDrawElements(int *pDatabuf);
int call_glFinish(int *pDatabuf);
int call_glFlush(int *pDatabuf);
int call_glFramebufferRenderbuffer(int *pDatabuf);
int call_glFramebufferTexture2D(int *pDatabuf);
int call_glFrontFace(int *pDatabuf);				//60

int call_glGenBuffers(int *pDatabuf);
int call_glGenerateMipmap(int *pDatabuf);
int call_glGenFramebuffers(int *pDatabuf);
int call_glGenRenderbuffers(int *pDatabuf);
int call_glGenTextures(int *pDatabuf);
int call_glGetActiveAttrib(int *pDatabuf);
int call_glGetActiveUniform(int *pDatabuf);
int call_glGetAttachedShaders(int *pDatabuf);
int call_glGetAttribLocation(int *pDatabuf);
int call_glGetBooleanv(int *pDatabuf);			//70

int call_glGetBufferParameteriv(int *pDatabuf);
int call_glGetError(int *pDatabuf);
int call_glGetFloatv(int *pDatabuf);
int call_glGetFramebufferAttachmentParameteriv(int *pDatabuf);
int call_glGetIntegerv(int *pDatabuf);
int call_glGetRenderbufferParameteriv(int *pDatabuf);
int call_glGetShaderPrecisionFormat(int *pDatabuf);
int call_glGetShaderSource(int *pDatabuf);
int call_glGetString(int *pDatabuf);
int call_glGetTexParameterfv(int *pDatabuf);		//80

int call_glGetTexParameteriv(int *pDatabuf);
int call_glGetUniformfv(int *pDatabuf);
int call_glGetUniformiv(int *pDatabuf);
int call_glGetVertexAttribfv(int *pDatabuf);
int call_glGetVertexAttribiv(int *pDatabuf);
int call_glGetVertexAttribPointerv(int *pDatabuf);
int call_glHint(int *pDatabuf);
int call_glIsBuffer(int *pDatabuf);
int call_glIsEnabled(int *pDatabuf);
int call_glIsFramebuffer(int *pDatabuf);			//90

int call_glIsProgram(int *pDatabuf);
int call_glIsRenderbuffer(int *pDatabuf);
int call_glIsShader(int *pDatabuf);
int call_glIsTexture(int *pDatabuf);
int call_glLineWidth(int *pDatabuf);
int call_glPixelStorei(int *pDatabuf);
int call_glPolygonOffset(int *pDatabuf);
int call_glReadPixels(int *pDatabuf);
int call_glReleaseShaderCompiler(int *pDatabuf);
int call_glRenderbufferStorage(int *pDatabuf);	//100

int call_glSampleCoverage(int *pDatabuf);
int call_glScissor(int *pDatabuf);
int call_glShaderBinary(int *pDatabuf);
int call_glStencilFunc(int *pDatabuf);
int call_glStencilFuncSeparate(int *pDatabuf);
int call_glStencilMask(int *pDatabuf);
int call_glStencilMaskSeparate(int *pDatabuf);
int call_glStencilOp(int *pDatabuf);
int call_glStencilOpSeparate(int *pDatabuf);
int call_glTexImage2D(int *pDatabuf);			//110

int call_glTexParameterf(int *pDatabuf);
int call_glTexParameterfv(int *pDatabuf);
int call_glTexParameteri(int *pDatabuf);
int call_glTexParameteriv(int *pDatabuf);
int call_glTexSubImage2D(int *pDatabuf);
int call_glUniform1f(int *pDatabuf);
int call_glUniform1fv(int *pDatabuf);
int call_glUniform1i(int *pDatabuf);
int call_glUniform1iv(int *pDatabuf);
int call_glUniform2f(int *pDatabuf);				//120

int call_glUniform2fv(int *pDatabuf);
int call_glUniform2i(int *pDatabuf);
int call_glUniform2iv(int *pDatabuf);
int call_glUniform3f(int *pDatabuf);
int call_glUniform3fv(int *pDatabuf);
int call_glUniform3i(int *pDatabuf);
int call_glUniform3iv(int *pDatabuf);
int call_glUniform4f(int *pDatabuf);
int call_glUniform4fv(int *pDatabuf);
int call_glUniform4i(int *pDatabuf);				//130

int call_glUniform4iv(int *pDatabuf);
int call_glUniformMatrix2fv(int *pDatabuf);
int call_glUniformMatrix3fv(int *pDatabuf);
int call_glValidateProgram(int *pDatabuf);
int call_glVertexAttrib1f(int *pDatabuf);
int call_glVertexAttrib1fv(int *pDatabuf);
int call_glVertexAttrib2f(int *pDatabuf);
int call_glVertexAttrib2fv(int *pDatabuf);
int call_glVertexAttrib3f(int *pDatabuf);
int call_glVertexAttrib3fv(int *pDatabuf);		//140

int call_glVertexAttrib4f(int *pDatabuf);
int call_glVertexAttrib4fv(int *pDatabuf);

/* EGL functions */
int call_eglGetDisplay(int *pDatabuf);
int call_eglInitialize(int *pDatabuf);
int call_eglBindAPI(int *pDatabuf);
int call_eglChooseConfig(int *pDatabuf);
int call_eglGetError(int *pDatabuf);
int call_eglCreateWindowSurface(int *pDatabuf);
int call_eglCreateContext(int *pDatabuf);
int call_eglMakeCurrent(int *pDatabuf);				//150
int call_eglDestroyContext(int *pDatabuf);
int call_eglDestroySurface(int *pDatabuf);
int call_eglTerminate (int *pDatabuf);
int call_eglReleaseThread(int *pDatabuf);	// the last function
int call_eglQueryString(int *pDatabuf);
int call_eglQuerySurface(int *pDatabuf);
int call_eglSwapBuffers(int *pDatabuf);
int call_eglGetConfigs(int *pDatabuf);
int call_eglGetConfigAttrib(int *pDatabuf);
int call_eglCreatePbufferSurface(int *pDatabuf);	//160
int call_eglCreatePixmapSurface(int *pDatabuf);
int call_eglQueryAPI(int *pDatabuf);
int call_eglWaitClient(int *pDatabuf);
int call_eglCreatePbufferFromClientBuffer(int *pDatabuf);
int call_eglSurfaceAttrib(int *pDatabuf);
int call_eglBindTexImage(int *pDatabuf);
int call_eglReleaseTexImage(int *pDatabuf);
int call_eglSwapInterval(int *pDatabuf);
int call_eglGetCurrentContext(int *pDatabuf);
int call_eglGetCurrentSurface(int *pDatabuf);	//170
int call_eglGetCurrentDisplay(int *pDatabuf);
int call_eglQueryContext(int *pDatabuf);
int call_eglWaitGL(int *pDatabuf);
int call_eglWaitNative(int *pDatabuf);
int call_eglCopyBuffers(int *pDatabuf);

/* vivante driver functions */
int call_fbGetDisplayByIndex(int *pDatabuf);
int call_fbCreateWindow(int *pDatabuf);
int call_fbGetDisplay(int *pDatabuf);
int call_fbGetDisplayGeometry(int *pDatabuf);
int call_fbGetDisplayInfo(int *pDatabuf);			//180
int call_fbDestroyDisplay(int *pDatabuf);
int call_fbGetWindowGeometry(int *pDatabuf);
int call_fbGetWindowInfo(int *pDatabuf);
int call_fbDestroyWindow(int *pDatabuf);
int call_fbCreatePixmap(int *pDatabuf);
int call_fbCreatePixmapWithBpp(int *pDatabuf);
int call_fbGetPixmapGeometry(int *pDatabuf);
int call_fbGetPixmapInfo(int *pDatabuf);
int call_fbDestroyPixmap(int *pDatabuf);			//189

