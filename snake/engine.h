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
#include "actor.h"

typedef struct engine engine;

typedef enum DIRECTION
{
    DIRECTION_N,
    DIRECTION_E,
    DIRECTION_S,
    DIRECTION_W
} DIRECTION;

enum
{
    SAND_TEXTURE,
    SPRITES_TEXTURE,
    NUM_TEXTURES
};

typedef enum SPRITES_DECALS
{
    TAIL_S,
    TAIL_W,
    HEAD_N,
    HEAD_E,
    BODY_SE,
    BODY_SW,
    BODY_NW,
    BLANK,
    TAIL_E,
    TAIL_N,
    HEAD_W,
    HEAD_S,
    BODY_N,
    BODY_E,
    BODY_NE,
    APPLE,
    NUM_SPRITES_DECALS
} SPRITES_DECALS;

enum {
    MAX_SNAKE_SEGMENTS = 8
};

#include "apple_actor.h"
#include "snake_actor.h"
#include "background_actor.h"

struct engine 
{
    unsigned int fps;
    unsigned int current_frame;
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
    GLint texture_sampler_unfm;

    GLint pos_attrib;
    GLint uv_attrib;

    actor_list *render_list;
    actor_list *logic_list;

    GLuint textures[NUM_TEXTURES];
    decal sprites_decals[NUM_SPRITES_DECALS];
    decal sand_decal;

    apple_actor apple_actor;
    background_actor background_actor;
    snake_actor snake_actor;
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
