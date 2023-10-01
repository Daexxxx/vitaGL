/*
 * This file is part of vitaGL
 * Copyright 2017, 2018, 2019, 2020 Rinnegatamante
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * lookup.c:
 * A GL GetProcAddress implementation
 */

#include "shared.h"
#include "vitaGL.h"

//#define EXPOSE_VGL_FUNCS // Define this to enable exposure of vgl* functions in vglGetProcAddress/eglGetProcAddress

static const struct {
	const char *name;
	void *proc;
} vgl_proctable[] = {
	// *gl
	{"glActiveTexture", (void *)glActiveTexture},
	{"glAlphaFunc", (void *)glAlphaFunc},
	{"glAlphaFuncx", (void *)glAlphaFuncx},
	{"glAttachShader", (void *)glAttachShader},
	{"glBegin", (void *)glBegin},
	{"glBindAttribLocation", (void *)glBindAttribLocation},
	{"glBindBuffer", (void *)glBindBuffer},
	{"glBindFramebuffer", (void *)glBindFramebuffer},
	{"glBindRenderbuffer", (void *)glBindRenderbuffer},
	{"glBindSampler", (void *)glBindSampler},
	{"glBindTexture", (void *)glBindTexture},
	{"glBindVertexArray", (void *)glBindVertexArray},
	{"glBlendEquation", (void *)glBlendEquation},
	{"glBlendEquationSeparate", (void *)glBlendEquationSeparate},
	{"glBlendFunc", (void *)glBlendFunc},
	{"glBlendFuncSeparate", (void *)glBlendFuncSeparate},
	{"glBlitFramebuffer", (void *)glBlitFramebuffer},
	{"glBlitNamedFramebuffer", (void *)glBlitNamedFramebuffer},
	{"glBufferData", (void *)glBufferData},
	{"glBufferSubData", (void *)glBufferSubData},
	{"glCallList", (void *)glCallList},
	{"glCallLists", (void *)glCallLists},
	{"glCheckFramebufferStatus", (void *)glCheckFramebufferStatus},
	{"glCheckNamedFramebufferStatus", (void *)glCheckNamedFramebufferStatus},
	{"glClear", (void *)glClear},
	{"glClearColor", (void *)glClearColor},
	{"glClearColorx", (void *)glClearColorx},
	{"glClearDepth", (void *)glClearDepth},
	{"glClearDepthf", (void *)glClearDepthf},
	{"glClearDepthx", (void *)glClearDepthx},
	{"glClearStencil", (void *)glClearStencil},
	{"glClientActiveTexture", (void *)glClientActiveTexture},
	{"glClipPlane", (void *)glClipPlane},
	{"glClipPlanef", (void *)glClipPlanef},
	{"glClipPlanex", (void *)glClipPlanex},
	{"glColor3f", (void *)glColor3f},
	{"glColor3fv", (void *)glColor3fv},
	{"glColor3ub", (void *)glColor3ub},
	{"glColor3ubv", (void *)glColor3ubv},
	{"glColor4f", (void *)glColor4f},
	{"glColor4fv", (void *)glColor4fv},
	{"glColor4ub", (void *)glColor4ub},
	{"glColor4ubv", (void *)glColor4ubv},
	{"glColor4x", (void *)glColor4x},
	{"glColorMask", (void *)glColorMask},
	{"glColorMaterial", (void *)glColorMaterial},
	{"glColorPointer", (void *)glColorPointer},
	{"glColorTable", (void *)glColorTable},
	{"glCompileShader", (void *)glCompileShader},
	{"glCompressedTexImage2D", (void *)glCompressedTexImage2D},
	{"glCopyTexImage2D", (void *)glCopyTexImage2D},
	{"glCopyTexSubImage2D", (void *)glCopyTexSubImage2D},
	{"glCreateProgram", (void *)glCreateProgram},
	{"glCreateShader", (void *)glCreateShader},
	{"glCullFace", (void *)glCullFace},
	{"glDeleteBuffers", (void *)glDeleteBuffers},
	{"glDeleteFramebuffers", (void *)glDeleteFramebuffers},
	{"glDeleteLists", (void *)glDeleteLists},
	{"glDeleteProgram", (void *)glDeleteProgram},
	{"glDeleteRenderbuffers", (void *)glDeleteRenderbuffers},
	{"glDeleteSamplers", (void *)glDeleteSamplers},
	{"glDeleteShader", (void *)glDeleteShader},
	{"glDeleteTextures", (void *)glDeleteTextures},
	{"glDeleteVertexArrays", (void *)glDeleteVertexArrays},
	{"glDepthFunc", (void *)glDepthFunc},
	{"glDepthMask", (void *)glDepthMask},
	{"glDepthRange", (void *)glDepthRange},
	{"glDepthRangef", (void *)glDepthRangef},
	{"glDepthRangex", (void *)glDepthRangex},
	{"glDisable", (void *)glDisable},
	{"glDisableClientState", (void *)glDisableClientState},
	{"glDisableVertexAttribArray", (void *)glDisableVertexAttribArray},
	{"glDrawArrays", (void *)glDrawArrays},
	{"glDrawArraysInstanced", (void *)glDrawArraysInstanced},
	{"glDrawElements", (void *)glDrawElements},
	{"glDrawElementsBaseVertex", (void *)glDrawElementsBaseVertex},
	{"glDrawElementsInstanced", (void *)glDrawElementsInstanced},
	{"glDrawRangeElements", (void *)glDrawRangeElements},
	{"glDrawRangeElementsBaseVertex", (void *)glDrawRangeElementsBaseVertex},
	{"glEnable", (void *)glEnable},
	{"glEnableClientState", (void *)glEnableClientState},
	{"glEnableVertexAttribArray", (void *)glEnableVertexAttribArray},
	{"glEnd", (void *)glEnd},
	{"glEndList", (void *)glEndList},
	{"glFinish", (void *)glFinish},
	{"glFlush", (void *)glFlush},
	{"glFlushMappedBufferRange", (void *)glFlushMappedBufferRange},
	{"glFogf", (void *)glFogf},
	{"glFogfv", (void *)glFogfv},
	{"glFogi", (void *)glFogi},
	{"glFogx", (void *)glFogx},
	{"glFogxv", (void *)glFogxv},
	{"glFramebufferRenderbuffer", (void *)glFramebufferRenderbuffer},
	{"glFramebufferTexture", (void *)glFramebufferTexture},
	{"glFramebufferTexture2D", (void *)glFramebufferTexture2D},
	{"glFrontFace", (void *)glFrontFace},
	{"glFrustum", (void *)glFrustum},
	{"glFrustumf", (void *)glFrustumf},
	{"glFrustumx", (void *)glFrustumx},
	{"glGenBuffers", (void *)glGenBuffers},
	{"glGenerateMipmap", (void *)glGenerateMipmap},
	{"glGenerateTextureMipmap", (void *)glGenerateTextureMipmap},
	{"glGenFramebuffers", (void *)glGenFramebuffers},
	{"glGenLists", (void *)glGenLists},
	{"glGenRenderbuffers", (void *)glGenRenderbuffers},
	{"glGenSamplers", (void *)glGenSamplers},
	{"glGenTextures", (void *)glGenTextures},
	{"glGenVertexArrays", (void *)glGenVertexArrays},
	{"glGetActiveAttrib", (void *)glGetActiveAttrib},
	{"glGetActiveUniform", (void *)glGetActiveUniform},
	{"glGetAttachedShaders", (void *)glGetAttachedShaders},
	{"glGetAttribLocation", (void *)glGetAttribLocation},
	{"glGetBooleanv", (void *)glGetBooleanv},
	{"glGetBufferParameteriv", (void *)glGetBufferParameteriv},
	{"glGetDoublev", (void *)glGetDoublev},
	{"glGetError", (void *)glGetError},
	{"glGetFloatv", (void *)glGetFloatv},
	{"glGetFramebufferAttachmentParameteriv", (void *)glGetFramebufferAttachmentParameteriv},
	{"glGetIntegerv", (void *)glGetIntegerv},
	{"glGetPointerv", (void *)glGetPointerv},
	{"glGetProgramBinary", (void *)glGetProgramBinary},
	{"glGetProgramInfoLog", (void *)glGetProgramInfoLog},
	{"glGetProgramiv", (void *)glGetProgramiv},
	{"glGetShaderInfoLog", (void *)glGetShaderInfoLog},
	{"glGetShaderiv", (void *)glGetShaderiv},
	{"glGetShaderSource", (void *)glGetShaderSource},
	{"glGetString", (void *)glGetString},
	{"glGetStringi", (void *)glGetStringi},
	{"glGetTexEnviv", (void *)glGetTexEnviv},
	{"glGetUniformLocation", (void *)glGetUniformLocation},
	{"glGetVertexAttribfv", (void *)glGetVertexAttribfv},
	{"glGetVertexAttribiv", (void *)glGetVertexAttribiv},
	{"glGetVertexAttribPointerv", (void *)glGetVertexAttribPointerv},
	{"glHint", (void *)glHint},
	{"glInterleavedArrays", (void *)glInterleavedArrays},
	{"glIsEnabled", (void *)glIsEnabled},
	{"glIsFramebuffer", (void *)glIsFramebuffer},
	{"glIsProgram", (void *)glIsProgram},
	{"glIsRenderbuffer", (void *)glIsRenderbuffer},
	{"glIsTexture", (void *)glIsTexture},
	{"glLightfv", (void *)glLightfv},
	{"glLightModelfv", (void *)glLightModelfv},
	{"glLightModelxv", (void *)glLightModelxv},
	{"glLightxv", (void *)glLightxv},
	{"glLineWidth", (void *)glLineWidth},
	{"glLineWidthx", (void *)glLineWidthx},
	{"glLinkProgram", (void *)glLinkProgram},
	{"glListBase", (void *)glListBase},
	{"glLoadIdentity", (void *)glLoadIdentity},
	{"glLoadMatrixf", (void *)glLoadMatrixf},
	{"glLoadMatrixx", (void *)glLoadMatrixx},
	{"glLoadTransposeMatrixf", (void *)glLoadTransposeMatrixf},
	{"glLoadTransposeMatrixx", (void *)glLoadTransposeMatrixx},
	{"glMapBuffer", (void *)glMapBuffer},
	{"glMapBufferRange", (void *)glMapBufferRange},
	{"glMaterialf", (void *)glMaterialf},
	{"glMaterialfv", (void *)glMaterialfv},
	{"glMateriali", (void *)glMateriali},
	{"glMaterialx", (void *)glMaterialx},
	{"glMaterialxv", (void *)glMaterialxv},
	{"glMatrixMode", (void *)glMatrixMode},
	{"glMultiTexCoord2f", (void *)glMultiTexCoord2f},
	{"glMultiTexCoord2fv", (void *)glMultiTexCoord2fv},
	{"glMultiTexCoord2i", (void *)glMultiTexCoord2i},
	{"glMultMatrixf", (void *)glMultMatrixf},
	{"glMultMatrixx", (void *)glMultMatrixx},
	{"glMultTransposeMatrixf", (void *)glMultTransposeMatrixf},
	{"glMultTransposeMatrixx", (void *)glMultTransposeMatrixx},
	{"glNamedFramebufferRenderbuffer", (void *)glNamedFramebufferRenderbuffer},
	{"glNamedFramebufferTexture", (void *)glNamedFramebufferTexture},
	{"glNamedFramebufferTexture2D", (void *)glNamedFramebufferTexture2D},
	{"glNamedRenderbufferStorage", (void *)glNamedRenderbufferStorage},
	{"glNewList", (void *)glNewList},
	{"glNormal3f", (void *)glNormal3f},
	{"glNormal3fv", (void *)glNormal3fv},
	{"glNormal3s", (void *)glNormal3s},
	{"glNormal3x", (void *)glNormal3x},
	{"glOrtho", (void *)glOrtho},
	{"glOrthof", (void *)glOrthof},
	{"glOrthox", (void *)glOrthox},
	{"glPixelStorei", (void *)glPixelStorei},
	{"glPointSize", (void *)glPointSize},
	{"glPointSizex", (void *)glPointSizex},
	{"glPolygonMode", (void *)glPolygonMode},
	{"glPolygonOffset", (void *)glPolygonOffset},
	{"glPolygonOffsetx", (void *)glPolygonOffsetx},
	{"glPopAttrib", (void *)glPopAttrib},
	{"glPopMatrix", (void *)glPopMatrix},
	{"glProgramBinary", (void *)glProgramBinary},
	{"glProgramUniform1f", (void *)glProgramUniform1f},
	{"glProgramUniform1fv", (void *)glProgramUniform1fv},
	{"glProgramUniform1i", (void *)glProgramUniform1i},
	{"glProgramUniform1iv", (void *)glProgramUniform1iv},
	{"glProgramUniform2f", (void *)glProgramUniform2f},
	{"glProgramUniform2fv", (void *)glProgramUniform2fv},
	{"glProgramUniform2i", (void *)glProgramUniform2i},
	{"glProgramUniform2iv", (void *)glProgramUniform2iv},
	{"glProgramUniform3f", (void *)glProgramUniform3f},
	{"glProgramUniform3fv", (void *)glProgramUniform3fv},
	{"glProgramUniform3i", (void *)glProgramUniform3i},
	{"glProgramUniform3iv", (void *)glProgramUniform3iv},
	{"glProgramUniform4f", (void *)glProgramUniform4f},
	{"glProgramUniform4fv", (void *)glProgramUniform4fv},
	{"glProgramUniform4i", (void *)glProgramUniform4i},
	{"glProgramUniform4iv", (void *)glProgramUniform4iv},
	{"glProgramUniformMatrix2fv", (void *)glProgramUniformMatrix2fv},
	{"glProgramUniformMatrix3fv", (void *)glProgramUniformMatrix3fv},
	{"glProgramUniformMatrix4fv", (void *)glProgramUniformMatrix4fv},
	{"glPushAttrib", (void *)glPushAttrib},
	{"glPushMatrix", (void *)glPushMatrix},
	{"glReadPixels", (void *)glReadPixels},
	{"glRectf", (void *)glRectf},
	{"glRecti", (void *)glRecti},
	{"glReleaseShaderCompiler", (void *)glReleaseShaderCompiler},
	{"glRenderbufferStorage", (void *)glRenderbufferStorage},
	{"glRotated", (void *)glRotated},
	{"glRotatef", (void *)glRotatef},
	{"glRotatex", (void *)glRotatex},
	{"glSamplerParameterf", (void *)glSamplerParameterf},
	{"glSamplerParameteri", (void *)glSamplerParameteri},
	{"glScaled", (void *)glScaled},
	{"glScalef", (void *)glScalef},
	{"glScalex", (void *)glScalex},
	{"glScissor", (void *)glScissor},
	{"glShadeModel", (void *)glShadeModel},
	{"glShaderBinary", (void *)glShaderBinary},
	{"glShaderSource", (void *)glShaderSource},
	{"glStencilFunc", (void *)glStencilFunc},
	{"glStencilFuncSeparate", (void *)glStencilFuncSeparate},
	{"glStencilMask", (void *)glStencilMask},
	{"glStencilMaskSeparate", (void *)glStencilMaskSeparate},
	{"glStencilOp", (void *)glStencilOp},
	{"glStencilOpSeparate", (void *)glStencilOpSeparate},
	{"glTexCoord2f", (void *)glTexCoord2f},
	{"glTexCoord2fv", (void *)glTexCoord2fv},
	{"glTexCoord2i", (void *)glTexCoord2i},
	{"glTexCoord2iv", (void *)glTexCoord2i},
	{"glTexCoord2s", (void *)glTexCoord2s},
	{"glTexCoordPointer", (void *)glTexCoordPointer},
	{"glTexEnvf", (void *)glTexEnvf},
	{"glTexEnvfv", (void *)glTexEnvfv},
	{"glTexEnvi", (void *)glTexEnvi},
	{"glTexEnvx", (void *)glTexEnvx},
	{"glTexEnvxv", (void *)glTexEnvxv},
	{"glTexImage2D", (void *)glTexImage2D},
	{"glTexParameterf", (void *)glTexParameterf},
	{"glTexParameteri", (void *)glTexParameteri},
	{"glTexParameteriv", (void *)glTexParameteriv},
	{"glTexParameterx", (void *)glTexParameterx},
	{"glTexSubImage2D", (void *)glTexSubImage2D},
	{"glTranslated", (void *)glTranslated},
	{"glTranslatef", (void *)glTranslatef},
	{"glTranslatex", (void *)glTranslatex},
	{"glUniform1f", (void *)glUniform1f},
	{"glUniform1fv", (void *)glUniform1fv},
	{"glUniform1i", (void *)glUniform1i},
	{"glUniform1iv", (void *)glUniform1iv},
	{"glUniform2f", (void *)glUniform2f},
	{"glUniform2fv", (void *)glUniform2fv},
	{"glUniform2i", (void *)glUniform2i},
	{"glUniform2iv", (void *)glUniform2iv},
	{"glUniform3f", (void *)glUniform3f},
	{"glUniform3fv", (void *)glUniform3fv},
	{"glUniform3i", (void *)glUniform3i},
	{"glUniform3iv", (void *)glUniform3iv},
	{"glUniform4f", (void *)glUniform4f},
	{"glUniform4fv", (void *)glUniform4fv},
	{"glUniform4i", (void *)glUniform4i},
	{"glUniform4iv", (void *)glUniform4iv},
	{"glUniformMatrix2fv", (void *)glUniformMatrix2fv},
	{"glUniformMatrix3fv", (void *)glUniformMatrix3fv},
	{"glUniformMatrix4fv", (void *)glUniformMatrix4fv},
	{"glUnmapBuffer", (void *)glUnmapBuffer},
	{"glUseProgram", (void *)glUseProgram},
	{"glVertex2f", (void *)glVertex2f},
	{"glVertex2i", (void *)glVertex2i},
	{"glVertex2fv", (void *)glVertex2fv},
	{"glVertex3f", (void *)glVertex3f},
	{"glVertex3fv", (void *)glVertex3fv},
	{"glVertex3i", (void *)glVertex3i},
	{"glVertexAttrib1f", (void *)glVertexAttrib1f},
	{"glVertexAttrib1fv", (void *)glVertexAttrib1fv},
	{"glVertexAttrib2f", (void *)glVertexAttrib2f},
	{"glVertexAttrib2fv", (void *)glVertexAttrib2fv},
	{"glVertexAttrib3f", (void *)glVertexAttrib3f},
	{"glVertexAttrib3fv", (void *)glVertexAttrib3fv},
	{"glVertexAttrib4f", (void *)glVertexAttrib4f},
	{"glVertexAttrib4fv", (void *)glVertexAttrib4fv},
	{"glVertexAttribPointer", (void *)glVertexAttribPointer},
	{"glVertexPointer", (void *)glVertexPointer},
	{"glViewport", (void *)glViewport},
	// *glu
	{"gluBuild2DMipmaps", (void *)gluBuild2DMipmaps},
	{"gluLookAt", (void *)gluLookAt},
	{"gluPerspective", (void *)gluPerspective},
#ifdef EXPOSE_VGL_FUNCS
	// *vgl
	{"vglAddSemanticBinding", (void *)vglAddSemanticBinding},
	{"vglColorPointer", (void *)vglColorPointer},
	{"vglColorPointerMapped", (void *)vglColorPointerMapped},
	{"vglDrawObjects", (void *)vglDrawObjects},
	{"vglIndexPointer", (void *)vglIndexPointer},
	{"vglIndexPointerMapped", (void *)vglIndexPointerMapped},
	{"vglTexCoordPointer", (void *)vglTexCoordPointer},
	{"vglTexCoordPointerMapped", (void *)vglTexCoordPointerMapped},
	{"vglVertexPointer", (void *)vglVertexPointer},
	{"vglVertexPointerMapped", (void *)vglVertexPointerMapped},
	{"vglBindAttribLocation", (void *)vglBindAttribLocation},
	{"vglBindPackedAttribLocation", (void *)vglBindPackedAttribLocation},
	{"vglVertexAttribPointer", (void *)vglVertexAttribPointer},
	{"vglVertexAttribPointerMapped", (void *)vglVertexAttribPointerMapped},
	{"vglAlloc", (void *)vglAlloc},
	{"vglCalloc", (void *)vglCalloc},
	{"vglCgShaderSource", (void *)vglCgShaderSource},
	{"vglEnd", (void *)vglEnd},
	{"vglForceAlloc", (void *)vglForceAlloc},
	{"vglFree", (void *)vglFree},
	{"vglGetFuncName", (void *)vglGetFuncName},
	{"vglGetGxmTexture", (void *)vglGetGxmTexture},
	{"vglGetProcAddress", (void *)vglGetProcAddress},
	{"vglGetShaderBinary", (void *)vglGetShaderBinary},
	{"vglGetTexDataPointer", (void *)vglGetTexDataPointer},
	{"vglInit", (void *)vglInit},
	{"vglInitExtended", (void *)vglInitExtended},
	{"vglInitWithCustomSizes", (void *)vglInitWithCustomSizes},
	{"vglInitWithCustomThreshold", (void *)vglInitWithCustomThreshold},
	{"vglMalloc", (void *)vglMalloc},
	{"vglMallocUsableSize", (void *)vglMallocUsableSize},
	{"vglMemalign", (void *)vglMemalign},
	{"vglMemFree", (void *)vglMemFree},
	{"vglMemTotal", (void *)vglMemTotal},
	{"vglOverloadTexDataPointer", (void *)vglOverloadTexDataPointer},
	{"vglRealloc", (void *)vglRealloc},
	{"vglSetDisplayCallback", (void *)vglSetDisplayCallback},
	{"vglSetFragmentBufferSize", (void *)vglSetFragmentBufferSize},
	{"vglSetParamBufferSize", (void *)vglSetParamBufferSize},
	{"vglSetSemanticBindingMode", (void *)vglSetSemanticBindingMode},
	{"vglSetUSSEBufferSize", (void *)vglSetUSSEBufferSize},
	{"vglSetVDMBufferSize", (void *)vglSetVDMBufferSize},
	{"vglSetVertexBufferSize", (void *)vglSetVertexBufferSize},
	{"vglSetVertexPoolSize", (void *)vglSetVertexPoolSize},
	{"vglSetupGarbageCollector", (void *)vglSetupGarbageCollector},
	{"vglSetupRuntimeShaderCompiler", (void *)vglSetupRuntimeShaderCompiler},
	{"vglSwapBuffers", (void *)vglSwapBuffers},
	{"vglTexImageDepthBuffer", (void *)vglTexImageDepthBuffer},
	{"vglUseCachedMem", (void *)vglUseCachedMem},
	{"vglUseLowPrecision", (void *)vglUseLowPrecision},
	{"vglUseTripleBuffering", (void *)vglUseTripleBuffering},
	{"vglUseVram", (void *)vglUseVram},
	{"vglUseVramForUSSE", (void *)vglUseVramForUSSE},
	{"vglUseExtraMem", (void *)vglUseExtraMem},
	{"vglWaitVblankStart", (void *)vglWaitVblankStart},
#endif
	// *egl
	{"eglBindAPI", (void *)eglBindAPI},
	{"eglGetDisplay", (void *)eglGetDisplay},
	{"eglGetError", (void *)eglGetError},
	{"eglGetProcAddress", (void *)eglGetProcAddress},
	{"eglGetSystemTimeFrequencyNV", (void *)eglGetSystemTimeFrequencyNV},
	{"eglGetSystemTimeNV", (void *)eglGetSystemTimeNV},
	{"eglQueryAPI", (void *)eglQueryAPI},
	{"eglSwapInterval", (void *)eglSwapInterval},
	{"eglSwapBuffers", (void *)eglSwapBuffers},
};

static const size_t vgl_numproc = sizeof(vgl_proctable) / sizeof(*vgl_proctable);

void *vglGetProcAddress(const char *name) {
	if (!name || !*name) {
		return NULL;
	}

	// strip any extension markers
	const int len = strlen(name);
	char tmpname[len + 1];
	vgl_fast_memcpy(tmpname, name, len + 1);
	if (!strcmp(tmpname + len - 3, "EXT") || !strcmp(tmpname + len - 3, "ARB") || !strcmp(tmpname + len - 3, "OES")) {
		tmpname[len - 3] = 0;
	}

	// search for stripped name
	for (size_t i = 0; i < vgl_numproc; ++i) {
		if (!strcmp(tmpname, vgl_proctable[i].name)) {
			return vgl_proctable[i].proc;
		}
	}

#ifndef SKIP_ERROR_HANDLING
	vgl_log("%s:%d vglGetProcAddress: Requested an unimplemented function (%s).\n", __FILE__, __LINE__, name);
#endif
	return NULL;
}

void *vglGetFuncName(uint32_t func) {
	// search for function name
	for (size_t i = 0; i < vgl_numproc; ++i) {
		if (func == vgl_proctable[i].proc)
			return vgl_proctable[i].name;
	}
	
#ifndef SKIP_ERROR_HANDLING
	vgl_log("%s:%d vglGetFuncName: Requested an unimplemented function.\n", __FILE__, __LINE__);
#endif
	return NULL;
}
