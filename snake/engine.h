#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "input_processor.h"
#include "sprite.h"

typedef struct engine engine;

struct engine 
{
    unsigned int fps;
    unsigned int current_frame;
    unsigned int frames_last_render_loop;
    SDL_Window *window;
    unsigned int w;
    unsigned int h;
    SDL_GLContext main_context;

    Uint32 start_time;
    bool should_start_logic_loop;
    unsigned int whole_frames_to_do;

    // vertex buffer object for the square that is 
    // used for drawing sprites
    GLuint sprite_vertex_bo;

    // shader program for shading the sprites displayed
    // by the engine
    GLuint shader_program;

    GLint w_unfm;
    GLint h_unfm;
    GLint r_unfm;

    sprite_list *render_list;
};

engine eng;

engine *engine_init(
        unsigned int w,
        unsigned int h,
        const char *window_title,
        unsigned int fps);

void engine_destroy();

void engine_start();

#endif
