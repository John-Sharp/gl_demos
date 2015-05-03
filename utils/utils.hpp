#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

void init_sdl_gl_window(int w, int h, const char*title);

GLuint load_texture(const char *fname);

void swap_sdl_gl_window();

void destroy_sdl_gl_window();

#endif
