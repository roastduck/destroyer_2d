#include "loader.h"

void load_extensions()
{
    mypfnCreateShader = (PFNGLCREATESHADERPROC)
        glfwGetProcAddress("glCreateShader");
    
    mypfnShaderSource = (PFNGLSHADERSOURCEPROC)
        glfwGetProcAddress("glShaderSource");

    mypfnCompileShader = (PFNGLCOMPILESHADERPROC)
        glfwGetProcAddress("glCompileShader");

    mypfnGetShaderiv = (PFNGLGETSHADERIVPROC)
        glfwGetProcAddress("glGetShaderiv");

    mypfnGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)
        glfwGetProcAddress("glGetShaderInfoLog");

    mypfnCreateProgram = (PFNGLCREATEPROGRAMPROC)
        glfwGetProcAddress("glCreateProgram");

    mypfnAttachShader = (PFNGLATTACHSHADERPROC)
        glfwGetProcAddress("glAttachShader");

    mypfnLinkProgram = (PFNGLLINKPROGRAMPROC)
        glfwGetProcAddress("glLinkProgram");

    mypfnGetProgramiv = (PFNGLGETPROGRAMIVPROC)
        glfwGetProcAddress("glGetProgramiv");

    mypfnIsProgram = (PFNGLISPROGRAMPROC)
        glfwGetProcAddress("glIsProgram");

    mypfnUseProgram = (PFNGLUSEPROGRAMPROC)
        glfwGetProcAddress("glUseProgram");

    mypfnIsShader = (PFNGLISSHADERPROC)
        glfwGetProcAddress("glIsShader");

    mypfnGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)
        glfwGetProcAddress("glGetUniformLocation");

    mypfnUniform1i = (PFNGLUNIFORM1IPROC)
        glfwGetProcAddress("glUniform1i");

    mypfnBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)
        glfwGetProcAddress("glBlendFuncSeparate");

    mypfnGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)
        glfwGetProcAddress("glGenFramebuffers");

    mypfnDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)
        glfwGetProcAddress("glDeleteFramebuffers");

    mypfnBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)
        glfwGetProcAddress("glBindFramebuffer");

    mypfnFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)
        glfwGetProcAddress("glFramebufferTexture");

    mypfnDrawBuffers = (PFNGLDRAWBUFFERSPROC)
        glfwGetProcAddress("glDrawBuffers");

    mypfnCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)
        glfwGetProcAddress("glCheckFramebufferStatus");
}

PFNGLCREATESHADERPROC mypfnCreateShader;

PFNGLSHADERSOURCEPROC mypfnShaderSource;

PFNGLCOMPILESHADERPROC mypfnCompileShader;

PFNGLGETSHADERIVPROC mypfnGetShaderiv;

PFNGLGETSHADERINFOLOGPROC mypfnGetShaderInfoLog;

PFNGLCREATEPROGRAMPROC mypfnCreateProgram;

PFNGLATTACHSHADERPROC mypfnAttachShader;

PFNGLLINKPROGRAMPROC mypfnLinkProgram;

PFNGLGETPROGRAMIVPROC mypfnGetProgramiv;

PFNGLISPROGRAMPROC mypfnIsProgram;

PFNGLUSEPROGRAMPROC mypfnUseProgram;

PFNGLISSHADERPROC mypfnIsShader;

PFNGLGETUNIFORMLOCATIONPROC mypfnGetUniformLocation;

PFNGLUNIFORM1IPROC mypfnUniform1i;

PFNGLBLENDFUNCSEPARATEPROC mypfnBlendFuncSeparate;

PFNGLGENFRAMEBUFFERSPROC mypfnGenFramebuffers;

PFNGLDELETEFRAMEBUFFERSPROC mypfnDeleteFramebuffers;

PFNGLBINDFRAMEBUFFERPROC mypfnBindFramebuffer;

PFNGLFRAMEBUFFERTEXTUREPROC mypfnFramebufferTexture;

PFNGLDRAWBUFFERSPROC mypfnDrawBuffers;

PFNGLCHECKFRAMEBUFFERSTATUSPROC mypfnCheckFramebufferStatus;

