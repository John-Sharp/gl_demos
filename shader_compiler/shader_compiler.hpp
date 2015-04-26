#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H
#include <GL/gl.h>
GLuint compile_shader(
    const char *vertex_shader_path,
    const char *fragment_shader_path
);
#endif
