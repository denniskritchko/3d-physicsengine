#ifndef __glad_h_
#define __glad_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#ifndef GLAPI
#define GLAPI extern
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif

#ifndef APIENTRY
#define APIENTRY GLAPIENTRY
#endif

typedef void GLvoid;
typedef unsigned int GLenum;
typedef float GLfloat;
typedef int GLint;
typedef int GLsizei;
typedef unsigned int GLbitfield;
typedef double GLdouble;
typedef unsigned int GLuint;
typedef unsigned char GLboolean;
typedef unsigned char GLubyte;
typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;

#define GL_DEPTH_TEST                     0x0B71
#define GL_CULL_FACE                      0x0B44
#define GL_BACK                           0x0405
#define GL_CCW                            0x0901
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_TRIANGLES                      0x0004
#define GL_LINES                          0x0001
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_FALSE                          0x0
#define GL_TRUE                           0x1

typedef void (APIENTRY *PFNGLCLEARPROC) (GLbitfield mask);
typedef void (APIENTRY *PFNGLCLEARCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRY *PFNGLENABLEPROC) (GLenum cap);
typedef void (APIENTRY *PFNGLCULLFACEPROC) (GLenum mode);
typedef void (APIENTRY *PFNGLFRONTFACEPROC) (GLenum mode);
typedef void (APIENTRY *PFNGLVIEWPORTPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef GLuint (APIENTRY *PFNGLCREATESHADERPROC) (GLenum type);
typedef void (APIENTRY *PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (APIENTRY *PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (APIENTRY *PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *PFNGLDELETESHADERPROC) (GLuint shader);
typedef GLuint (APIENTRY *PFNGLCREATEPROGRAMPROC) (void);
typedef void (APIENTRY *PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY *PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRY *PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (APIENTRY *PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (APIENTRY *PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef void (APIENTRY *PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY *PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRY *PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY *PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY *PFNGLDRAWARRAYSPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY *PFNGLDRAWELEMENTSPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (APIENTRY *PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
typedef void (APIENTRY *PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef GLint (APIENTRY *PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRY *PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (APIENTRY *PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY *PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

GLAPI PFNGLCLEARPROC glClear;
GLAPI PFNGLCLEARCOLORPROC glClearColor;
GLAPI PFNGLENABLEPROC glEnable;
GLAPI PFNGLCULLFACEPROC glCullFace;
GLAPI PFNGLFRONTFACEPROC glFrontFace;
GLAPI PFNGLVIEWPORTPROC glViewport;
GLAPI PFNGLCREATESHADERPROC glCreateShader;
GLAPI PFNGLSHADERSOURCEPROC glShaderSource;
GLAPI PFNGLCOMPILESHADERPROC glCompileShader;
GLAPI PFNGLGETSHADERIVPROC glGetShaderiv;
GLAPI PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
GLAPI PFNGLDELETESHADERPROC glDeleteShader;
GLAPI PFNGLCREATEPROGRAMPROC glCreateProgram;
GLAPI PFNGLATTACHSHADERPROC glAttachShader;
GLAPI PFNGLLINKPROGRAMPROC glLinkProgram;
GLAPI PFNGLGETPROGRAMIVPROC glGetProgramiv;
GLAPI PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
GLAPI PFNGLDELETEPROGRAMPROC glDeleteProgram;
GLAPI PFNGLUSEPROGRAMPROC glUseProgram;
GLAPI PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
GLAPI PFNGLGENBUFFERSPROC glGenBuffers;
GLAPI PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
GLAPI PFNGLBINDBUFFERPROC glBindBuffer;
GLAPI PFNGLBUFFERDATAPROC glBufferData;
GLAPI PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
GLAPI PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
GLAPI PFNGLDRAWARRAYSPROC glDrawArrays;
GLAPI PFNGLDRAWELEMENTSPROC glDrawElements;
GLAPI PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
GLAPI PFNGLDELETEBUFFERSPROC glDeleteBuffers;
GLAPI PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
GLAPI PFNGLUNIFORM1FPROC glUniform1f;
GLAPI PFNGLUNIFORM3FPROC glUniform3f;
GLAPI PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

typedef void* (*GLADloadproc)(const char *name);
int gladLoadGLLoader(GLADloadproc load);

#ifdef __cplusplus
}
#endif

#endif 