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
#include "../shader_compiler/shader_compiler.hpp"
#include "../utils/utils.hpp"
#include "../InputProcessor/InputProcessor.hpp"
#include "hooke_cloth_eng.hpp"
#include "../utils/base_eng/base_eng.hpp"

#include "drawable.hpp"

enum { WIN_W = 800, WIN_H = 600, FPS = 100 };

int main() 
{
    SDL_Event event;
    BaseEng engine(WIN_W, WIN_H, "Hooke Cloth", FPS);
    bool carry_on = true;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint shader_program = compile_shader(
        "../shaders/basic_shading.vertexshader",
        "../shaders/basic_shading.fragmentshader");
    glUseProgram(shader_program);

    GLuint texture_id = load_texture("resources/cloth_texture.png");

    Drawable cloth_drawable(
        "cloth.bin",
        shader_program,
        vao,
        GL_TEXTURE0,
        texture_id);

    Cloth cloth(
        engine,
        5, // pts_w
        5, // pts_h
        cloth_drawable.ordered_positions,
        5, // g
        50, // hooke constant
        12, // damping constant
        1  // equilibrium length
    );

    cloth.add_fixed_pt(0, 4);
    cloth.add_fixed_pt(4, 4);

    InputProcessor in_processor(0.001, 0.001, glm::vec3(0.0, 0.0, 4));

    glm::vec3 light_position_worldspace = glm::vec3(2.0, 2.0, 2.0);
    GLuint light_pos_id = glGetUniformLocation(shader_program, "light_position_cameraspace");

    Uint32 last_time = SDL_GetTicks();
    Uint32 curr_time = SDL_GetTicks();
    while (carry_on) {
        // Work through all elapsed logic frames 
        while (engine.should_continue_logic_loops()) {
            cloth.calc_force();
            cloth.iterate();
        }
        curr_time = SDL_GetTicks();
        in_processor.compute_direction((double)(curr_time - last_time));
        last_time = curr_time;

        cloth_drawable.update_vertices(cloth.vertices);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = in_processor.get_view_mat();
        cloth_drawable.set_V(view);
        cloth_drawable.set_P(in_processor.get_proj_mat());

        glm::vec3 light_position_cameraspace(view * glm::vec4(light_position_worldspace, 1.0));
        glUniform3f(light_pos_id, light_position_cameraspace.x, light_position_cameraspace.y, light_position_cameraspace.z);

        cloth_drawable.draw();

#ifdef DEBUG_MODE
        glUseProgram(0);

        // Draw normals
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf((const GLfloat*)&projector[0]);
        glMatrixMode(GL_MODELVIEW);
        glm::mat4 MV = view * model;
        glLoadMatrixf((const GLfloat*)&MV[0]);

        glColor3f(0,0,1);
        glBegin(GL_LINES);
        for (unsigned int i=0; i<element_array.size(); i++){
            glm::vec3 p = vertices[element_array[i]];
            glVertex3fv(&p.x);
            glm::vec3 o = glm::normalize(normals[element_array[i]]);
            p+=o*0.1f;
            glVertex3fv(&p.x);
        }
        glEnd();

        // Draw line pointing to light position
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf((const GLfloat*)&view);
        glColor3f(1,0,0);
        glBegin(GL_LINES);

        glm::vec3 p = light_position_worldspace;
        glVertex3fv(&p.x);
        p = glm::vec3(0, 0, 0);
        glVertex3fv(&p.x);
        glEnd();

        glUseProgram(shader_program);
#endif
        SDL_GL_SwapWindow(engine.window);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                carry_on = false;
            }
            in_processor.process_event(&event);
        }

        GLenum err = glGetError();
        if (err != 0) {
            switch (err) {
                case GL_INVALID_ENUM:
                    fprintf(stderr, "invalid enum\n");
                    break;
                case GL_INVALID_VALUE:
                    fprintf(stderr, "invalid value\n");
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    fprintf(stderr, "invalid fb op\n");
                    break;
                case GL_INVALID_OPERATION:
                    fprintf(stderr, "invalid fb\n");
                    break;
                case GL_OUT_OF_MEMORY:
                    fprintf(stderr, "no memory\n");
                    break;
            }
        }
    }

    // Cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shader_program);
}
