/**
 * Some functions in OpenGL have different address
 * in different platforms. So they should be load
 * at runtime
 */

#ifndef LOADER_H_
#define LOADER_H_

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

void load_extensions();

#define glCreateShader mypfnCreateShader
extern PFNGLCREATESHADERPROC mypfnCreateShader;

#define glShaderSource mypfnShaderSource
extern PFNGLSHADERSOURCEPROC mypfnShaderSource;

#define glCompileShader mypfnCompileShader
extern PFNGLCOMPILESHADERPROC mypfnCompileShader;

#define glGetShaderiv mypfnGetShaderiv
extern PFNGLGETSHADERIVPROC mypfnGetShaderiv;

#define glGetShaderInfoLog mypfnGetShaderInfoLog
extern PFNGLGETSHADERINFOLOGPROC mypfnGetShaderInfoLog;

#define glCreateProgram mypfnCreateProgram
extern PFNGLCREATEPROGRAMPROC mypfnCreateProgram;

#define glAttachShader mypfnAttachShader
extern PFNGLATTACHSHADERPROC mypfnAttachShader;

#define glLinkProgram mypfnLinkProgram
extern PFNGLLINKPROGRAMPROC mypfnLinkProgram;

#define glGetProgramiv mypfnGetProgramiv
extern PFNGLGETPROGRAMIVPROC mypfnGetProgramiv;

#define glIsProgram mypfnIsProgram
extern PFNGLISPROGRAMPROC mypfnIsProgram;

#define glUseProgram mypfnUseProgram
extern PFNGLUSEPROGRAMPROC mypfnUseProgram;

#define glIsShader mypfnIsShader
extern PFNGLISSHADERPROC mypfnIsShader;

#define glGetUniformLocation mypfnGetUniformLocation
extern PFNGLGETUNIFORMLOCATIONPROC mypfnGetUniformLocation;

#define glUniform1i mypfnUniform1i
extern PFNGLUNIFORM1IPROC mypfnUniform1i;

#define glBlendFuncSeparate mypfnBlendFuncSeparate
extern PFNGLBLENDFUNCSEPARATEPROC mypfnBlendFuncSeparate;

#define glGenFramebuffers mypfnGenFramebuffers
extern PFNGLGENFRAMEBUFFERSPROC mypfnGenFramebuffers;

#define glBindFramebuffer mypfnBindFramebuffer
extern PFNGLBINDFRAMEBUFFERPROC mypfnBindFramebuffer;

#define glFramebufferTexture mypfnFramebufferTexture
extern PFNGLFRAMEBUFFERTEXTUREPROC mypfnFramebufferTexture;

#define glDrawBuffers mypfnDrawBuffers
extern PFNGLDRAWBUFFERSPROC mypfnDrawBuffers;

#define glCheckFramebufferStatus mypfnCheckFramebufferStatus
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC mypfnCheckFramebufferStatus;

#endif // LOADER_H_
