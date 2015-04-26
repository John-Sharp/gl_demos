#include <fstream>
#include <stdio.h>
#include <iterator>
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdlib.h>

GLuint compile_shader(
    const char *vertex_shader_path,
    const char *fragment_shader_path
) {
    std::ifstream vertex_file_stream(vertex_shader_path);
    std::string vertex_shader_source(
            (std::istreambuf_iterator<char>(vertex_file_stream)),
            std::istreambuf_iterator<char>());
    const char *vertex_shader_source_c_str = vertex_shader_source.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source_c_str, NULL);
    glCompileShader(vertex_shader);

    GLint status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, buffer);
        fprintf(stderr, "vertexshader compile error: %s\n", buffer);
        exit(1);
    }

    std::ifstream fragment_file_stream(fragment_shader_path);
    std::string fragment_shader_source(
            (std::istreambuf_iterator<char>(fragment_file_stream)),
            std::istreambuf_iterator<char>());
    const char *fragment_shader_source_c_str = fragment_shader_source.c_str();

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(
        fragment_shader,
        1,
        &fragment_shader_source_c_str,
        NULL);

    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(fragment_shader, 512, NULL, buffer);
        fprintf(stderr, "fragmentshader compile error: %s\n", buffer);
        exit(1);
    }

    GLuint shader_program =glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);

    return shader_program;
}
