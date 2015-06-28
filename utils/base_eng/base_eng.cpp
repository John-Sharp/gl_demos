#include "base_eng.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


bool BaseEng::should_continue_logic_loops()
{
    if (!this->start_time) {
        this->start_time = SDL_GetTicks();
    }

    if (should_start_logic_loop) {
        Uint32 logic_loop_start_time = SDL_GetTicks();
        double elapsed_frames = (double)(logic_loop_start_time \
                - this->start_time) / 1000.0f * this->fps;
        this->frames_last_render_loop = 0;
        this->whole_frames_to_do = (unsigned int)elapsed_frames - this->current_frame;
    }
    if (!this->whole_frames_to_do) {
        this->should_start_logic_loop = true;
        return false;
    }
        
    this->whole_frames_to_do -= 1;
    this->current_frame += 1;
    this->frames_last_render_loop += 1;
    this->should_start_logic_loop = false;
    return true;
}

BaseEng::BaseEng(
    int w,
    int h,
    const char *window_title,
    unsigned int fps,
    BaseInputProcessor *input_processor) :  
        fps(fps),
        current_frame(0),
        frames_last_render_loop(0),
        base_input_processor(input_processor),
        should_start_logic_loop(true),
        whole_frames_to_do(0),
        start_time(0)
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Video initialisation failed: %s\n", SDL_GetError());
        exit(1);
    }

#ifndef DEBUG_MODE
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

    this->window = SDL_CreateWindow(
            window_title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            w,
            h,
            SDL_WINDOW_OPENGL);

    if(window == NULL) {
        fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
        exit(1);
    }

    this->main_context = SDL_GL_CreateContext(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(1);
    }

    SDL_GL_SetSwapInterval(1);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

//    glEnable(GL_CULL_FACE);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

BaseEng::~BaseEng()
{
    SDL_GL_DeleteContext(this->main_context);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

