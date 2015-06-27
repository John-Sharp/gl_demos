#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

#include "../utils/base_eng/base_eng.hpp"
#include "../shader_compiler/shader_compiler.hpp"
#include "../utils/utils.hpp"
#include "../InputProcessor/gen_input_processor.hpp"
#include "boaler.hpp"
#include "../cameras/fp_camera.hpp"
#include "../billboard/billboard.hpp"
#include "../text_texture/text_texture.hpp"
#include "mo_billboard.hpp"
#include "mo_eng.hpp"

enum { WIN_W = 800, WIN_H = 600, FPS = 100 };

unsigned int direction_fn(const BaseEng &eng)
{
    unsigned int direction = CAM_MV_NONE;
    GenInputProcessor<game_states> *input_processor = static_cast<GenInputProcessor<game_states> *>(eng.base_input_processor);

    const MoEng &mo_eng = static_cast<const MoEng &>(eng);

    if (mo_eng.active_object) {
        return 0;
    }

    if (input_processor->is_state_active(MV_UP)) {
        direction |= CAM_MV_UP;
    }
    if (input_processor->is_state_active(MV_DOWN)) {
        direction |= CAM_MV_DOWN;
    }
    if (input_processor->is_state_active(MV_LEFT)) {
        direction |= CAM_MV_LEFT;
    }
    if (input_processor->is_state_active(MV_RIGHT)) {
        direction |= CAM_MV_RIGHT;
    }
    if (input_processor->is_state_active(MV_FORWARD)) {
        direction |= CAM_MV_FORWARD;
    }
    return direction;
}

struct texture_struct {
    unsigned int texture_unit_index;
    GLuint texture_id;
};

void change_texture(const BaseEng &eng, BoalerModelUnit &mu, const texture_struct *ts)
{
    GenInputProcessor<game_states> *input_processor = static_cast<GenInputProcessor<game_states> *>(eng.base_input_processor);

    static bool texture_should_change = true;
    static unsigned int i = 0;

    if (input_processor->is_state_active(CHANGE_TEXTURE) && texture_should_change) {
        i = (i + 1) % 2;
        mu.texture_unit_index = ts[i].texture_unit_index;
        mu.texture_id = ts[i].texture_id;
        texture_should_change = false;
        return;
    }

    if (!input_processor->is_state_active(CHANGE_TEXTURE)) {
        texture_should_change = true;
    }
}

int main()
{
    try {
        SDL_Event event;

        MoEng engine(
                "resources/test_setup.json",
                WIN_W,
                WIN_H,
                "Multi object draw",
                FPS);


        bool carry_on = true;

        FpCamera camera(
                engine,
                0.8,
                0.5,
                engine.initial_camera_pos,
                direction_fn);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        while(carry_on) {

            // Work through all elapsed logic frames 
            while (engine.should_continue_logic_loops()) {
                camera.compute_direction();
                engine.do_logic();
            }

            engine.view_unit.V = camera.get_V();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            engine.render();

            SDL_GL_SwapWindow(engine.window);

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    carry_on = false;
                }
                engine.process_input(&event);
            }
        }
    } catch (const MoEngReturnException& e)
    {
        return EXIT_FAILURE;
    }
}
