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

enum { WIN_W = 800, WIN_H = 600, FPS = 100 };

enum game_states { MV_UP, MV_DOWN, MV_LEFT, MV_RIGHT, MV_FORWARD,
    CHANGE_TEXTURE };

unsigned int direction_fn(const BaseEng &eng)
{
    unsigned int direction = CAM_MV_NONE;
    GenInputProcessor<game_states> *input_processor = static_cast<GenInputProcessor<game_states> *>(eng.input_processor);

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
    GenInputProcessor<game_states> *input_processor = static_cast<GenInputProcessor<game_states> *>(eng.input_processor);

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
    SDL_Event event;

    GenInputProcessor<game_states> input_processor;
    input_processor.add_key_binding(SDLK_UP, MV_UP);
    input_processor.add_key_binding(SDLK_DOWN, MV_DOWN);
    input_processor.add_key_binding(SDLK_LEFT, MV_LEFT);
    input_processor.add_key_binding(SDLK_RIGHT, MV_RIGHT);
    input_processor.add_key_binding(SDLK_a, MV_FORWARD);
    input_processor.add_key_binding(SDLK_s, CHANGE_TEXTURE);

    BaseEng engine(WIN_W, WIN_H, "Hooke Cloth", FPS, static_cast<BaseInputProcessor *>(&input_processor));
    bool carry_on = true;

    GLuint shader_program = compile_shader(
        "resources/basic_shading.vertexshader",
        "resources/basic_shading.fragmentshader");
    BoalerShaderUnit shader_unit(shader_program);

    texture_struct ts[2] = {
        { .texture_unit_index = 0, .texture_id = load_texture("resources/test_texture.png") },
        { .texture_unit_index = 1, .texture_id = load_texture("resources/test_texture2.png") }
    };

    glm::mat4 V = glm::lookAt(
        glm::vec3(0.0, 0.0, 4.0),
        glm::vec3(0.0, 0.0, -1.0),
        glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 P = glm::perspective(44.9f, 4.0f / 3.0f, 0.1f, 100.0f);

    BoalerViewUnit view_unit(V, P);

    BoalerModel model("triangle.bin");
    BoalerModelUnit model_unit(glm::mat4(), ts[0].texture_unit_index, ts[0].texture_id, model);

    BoalerVSLink vs_link(view_unit, shader_unit);
    BoalerVSLModelUnitLink vslm_link(vs_link, model_unit);

    MoBillboard::prep(shader_unit);
    MoBillboard bb(vslm_link, 1, 0.8);

    BoalerEng beng;
    beng.reg_view_unit(&view_unit);
    beng.reg_shader_unit(&shader_unit);
    beng.reg_model(&model);
    beng.reg_model_unit(&model_unit);

    beng.reg_view_unit(bb.view_unit);
    beng.reg_model(bb.model);
    beng.reg_model_unit(&bb.model_unit);

    FpCamera camera(
        engine,
        0.1,
        0.1,
        glm::vec3(0.0, 0.0, 4),
        direction_fn);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    while(carry_on) {

        // Work through all elapsed logic frames 
        while (engine.should_continue_logic_loops()) {
            camera.compute_direction();
        }

        view_unit.V = camera.get_V();
        change_texture(engine, model_unit, ts);

        bb.bb.update_pos();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        beng.render();
        SDL_GL_SwapWindow(engine.window);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                carry_on = false;
            }
            input_processor.process_input(&event);
        }
    }
}
