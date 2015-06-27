#ifndef BASE_ENG_H
#define BASE_ENG_H

#include <stdbool.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "../../InputProcessor/gen_input_processor.hpp"


class BaseEng
{
    public:
        SDL_Window *window;
        SDL_GLContext main_context;

        unsigned int fps;
        unsigned int current_frame;
        unsigned int frames_last_render_loop;

        bool should_continue_logic_loops();

        BaseInputProcessor *base_input_processor;

        BaseEng(
            int w,
            int h,
            const char *window_title,
            unsigned int fps,
            BaseInputProcessor *input_processor);
        ~BaseEng();



    private:
        bool should_start_logic_loop;
        unsigned int whole_frames_to_do;

        Uint32 start_time;

        void init_for_global_loop();
};

#endif
