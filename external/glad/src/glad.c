#include <glad/glad.h>
#include <string.h>
#include <stdio.h>

PFNGLCLEARPROC glClear;
PFNGLCLEARCOLORPROC glClearColor;
PFNGLENABLEPROC glEnable;
PFNGLCULLFACEPROC glCullFace;
PFNGLFRONTFACEPROC glFrontFace;
PFNGLVIEWPORTPROC glViewport;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDRAWARRAYSPROC glDrawArrays;
PFNGLDRAWELEMENTSPROC glDrawElements;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

int gladLoadGLLoader(GLADloadproc load) {
    if (load == NULL) {
        return 0;
    }

    glClear = (PFNGLCLEARPROC)load("glClear");
    glClearColor = (PFNGLCLEARCOLORPROC)load("glClearColor");
    glEnable = (PFNGLENABLEPROC)load("glEnable");
    glCullFace = (PFNGLCULLFACEPROC)load("glCullFace");
    glFrontFace = (PFNGLFRONTFACEPROC)load("glFrontFace");
    glViewport = (PFNGLVIEWPORTPROC)load("glViewport");
    glCreateShader = (PFNGLCREATESHADERPROC)load("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)load("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)load("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)load("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)load("glGetShaderInfoLog");
    glDeleteShader = (PFNGLDELETESHADERPROC)load("glDeleteShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)load("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)load("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)load("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)load("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)load("glGetProgramInfoLog");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)load("glDeleteProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC)load("glUseProgram");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)load("glGenVertexArrays");
    glGenBuffers = (PFNGLGENBUFFERSPROC)load("glGenBuffers");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)load("glBindVertexArray");
    glBindBuffer = (PFNGLBINDBUFFERPROC)load("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)load("glBufferData");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)load("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)load("glEnableVertexAttribArray");
    glDrawArrays = (PFNGLDRAWARRAYSPROC)load("glDrawArrays");
    glDrawElements = (PFNGLDRAWELEMENTSPROC)load("glDrawElements");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)load("glDeleteVertexArrays");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)load("glDeleteBuffers");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)load("glGetUniformLocation");
    glUniform1f = (PFNGLUNIFORM1FPROC)load("glUniform1f");
    glUniform3f = (PFNGLUNIFORM3FPROC)load("glUniform3f");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)load("glUniformMatrix4fv");

    return 1;
} 