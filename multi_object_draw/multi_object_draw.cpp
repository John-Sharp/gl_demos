#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

enum { WIN_W = 800, WIN_H = 600, FPS = 100 };

enum game_states { TEST_STATE1, TEST_STATE2, TEST_STATE3 };

int main()
{
    SDL_Event event;
    BaseEng engine(WIN_W, WIN_H, "Hooke Cloth", FPS);
    bool carry_on = true;

    GLuint shader_program = compile_shader(
        "resources/basic_shading.vertexshader",
        "resources/basic_shading.fragmentshader");
    BoalerShaderUnit shader_unit(shader_program);

    GLuint texture_id = load_texture("resources/test_texture.png");

    glm::mat4 V = glm::lookAt(
        glm::vec3(0.0, 0.0, 4.0),
        glm::vec3(0.0, 0.0, -1.0),
        glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 P = glm::perspective(44.9f, 4.0f / 3.0f, 0.1f, 100.0f);
    BoalerViewUnit view_unit(V, P);

    BoalerModel model("triangle.bin");
    BoalerModelUnit model_unit(glm::mat4(), 0, texture_id, model);

    BoalerVSLink vs_link(view_unit, shader_unit);
    BoalerVSLModelUnitLink vslm_link(vs_link, model_unit);

    BoalerEng beng;
    beng.reg_view_unit(&view_unit);
    beng.reg_shader_unit(&shader_unit);
    beng.reg_model(&model);
    beng.reg_model_unit(&model_unit);

    GenInputProcessor<game_states> input_processor;
    input_processor.add_key_binding(SDLK_DOWN, TEST_STATE1); 

    while(carry_on) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        beng.render();
        SDL_GL_SwapWindow(engine.window);

        if (input_processor.is_state_active(TEST_STATE1)) {
            fprintf(stderr, "state 1 active");
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                carry_on = false;
            }
            input_processor.process_input(&event);
        }
    }
}
