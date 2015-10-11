#ifndef ENGINE_H
#define ENGINE_H

#include "engine.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

engine eng;

engine *engine_init(
        unsigned int w,
        unsigned int h,
        const char *window_title,
        unsigned int fps)
{
    eng.fps = fps;
    eng.current_frame = 0;
    eng.frames_last_render_loop = 0;

    eng.start_time = 0;
    eng.should_start_logic_loop = false;
    // unsigned int whole_frames_to_do = 0;

    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(
                stderr, 
                "Video initialisation failed: %s\n", SDL_GetError());
        return NULL;
    }

    eng.window = SDL_CreateWindow(
            window_title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            w,
            h,
            SDL_WINDOW_OPENGL);

    if(eng.window == NULL) {
        fprintf(
                stderr,
                "Window could not be created: %s\n", SDL_GetError());
        return NULL;
    }

    eng.main_context = SDL_GL_CreateContext(eng.window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(1);
    }

    SDL_GL_SetSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return &eng;
}

void engine_destroy()
{
    SDL_GL_DeleteContext(eng.main_context);
    SDL_DestroyWindow(eng.window);
    SDL_Quit();
}

void engine_start()
{
    bool carry_on = true;

    while (carry_on) {
        process_input();

        if (is_state_active(GS_QUIT)) {
            carry_on = false;
        }
    }
}
#endif
