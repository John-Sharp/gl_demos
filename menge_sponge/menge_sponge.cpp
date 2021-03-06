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
#include "../simple_obj_reader/bin_obj_reader.hpp"
#include "../shader_compiler/shader_compiler.hpp"
#include "../utils/utils.hpp"
#include "../InputProcessor/InputProcessor.hpp"
#include "read_menge_grid.hpp"
#include <iostream>

#define DEBUG_MODE

int main() 
{
    SDL_Event event;
    init_sdl_gl_window(800, 600, "Menge Sponge");
    bool carry_on = true;
    std::vector<GLuint> element_array;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> ordered_vertices;
    std::vector<GLuint> element_array_vertices_ordered_vertices_map;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    bin_obj_read(
        "cube.bin", 
        element_array,
        vertices,
        uvs,
        normals,
        &ordered_vertices,
        &element_array_vertices_ordered_vertices_map);

    GLuint shader_program = compile_shader(
        "../shaders/basic_shading.vertexshader",
        "../shaders/basic_shading.fragmentshader");
    glUseProgram(shader_program);

    GLuint element_bo;
    glGenBuffers(1, &element_bo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_bo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        element_array.size() * sizeof(element_array[0]),
        &element_array[0],
        GL_STATIC_DRAW);

    GLuint vertex_bo, uv_bo, normal_bo;

    glGenBuffers(1, &vertex_bo);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_bo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(vertices[0]),
        &vertices[0][0],
        GL_STATIC_DRAW);

    GLint pos_attrib = glGetAttribLocation(shader_program, "position_modelspace");
    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(pos_attrib);

    glGenBuffers(1, &uv_bo);

    glBindBuffer(GL_ARRAY_BUFFER, uv_bo);
    glBufferData(
        GL_ARRAY_BUFFER,
        uvs.size() * sizeof(uvs[0]),
        &uvs[0][0],
        GL_STATIC_DRAW);
    GLint uv_attrib = glGetAttribLocation(shader_program, "uv");
    glVertexAttribPointer(uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(uv_attrib);

    glGenBuffers(1, &normal_bo);
    glBindBuffer(GL_ARRAY_BUFFER, normal_bo);
    glBufferData(
            GL_ARRAY_BUFFER,
            normals.size() * sizeof(normals[0]),
            &normals[0][0],
            GL_STATIC_DRAW);

    GLint normal_attrib = glGetAttribLocation(shader_program, "normal_modelspace");
    glVertexAttribPointer(normal_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normal_attrib);

    GLuint instance_model_bo;
    glGenBuffers(1, &instance_model_bo);


    std::vector<glm::vec3> instance_displacements;
    bin_menge_grid_read(
            "3d_menge_grid_displacements.bin",
            instance_displacements);
    std::cout << instance_displacements.size();

    glBindBuffer(GL_ARRAY_BUFFER, instance_model_bo);
    glBufferData(
        GL_ARRAY_BUFFER,
        instance_displacements.size() * sizeof(instance_displacements[0]),
        &instance_displacements[0][0],
        GL_STATIC_DRAW);

    GLint instance_displacement_attrib = glGetAttribLocation(shader_program, "instance_displacement");
    glEnableVertexAttribArray(instance_displacement_attrib);
    glVertexAttribPointer(
            instance_displacement_attrib,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,0);
    glVertexAttribDivisor(instance_displacement_attrib, 1);
    
    GLuint texture_id = load_texture("resources/cube_texture.png");

    GLuint sampler_id = glGetUniformLocation(
        shader_program,
        "texture_sampler");

    InputProcessor in_processor(0.001, 0.001, glm::vec3(0.0, 0.0, 4));

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = in_processor.get_view_mat();
    glm::mat4 projector = in_processor.get_proj_mat();

    glm::vec3 light_position_worldspace = glm::vec3(2.0, 2.0, 2.0);
    GLuint light_pos_id = glGetUniformLocation(shader_program, "light_position_cameraspace");
    glm::vec3 light_position_cameraspace(view * glm::vec4(light_position_worldspace, 1.0));

    GLuint V_id = glGetUniformLocation(shader_program, "V");
    glm::mat4 V = view;

    GLuint P_id = glGetUniformLocation(shader_program, "P");
    glm::mat4 P = projector;

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    // Set our sampler to use Texture Unit 0
    glUniform1i(sampler_id, GL_TEXTURE0);

    Uint32 curr_time = SDL_GetTicks();
    Uint32 last_time = SDL_GetTicks();
    while(carry_on) {
        curr_time = SDL_GetTicks();
        in_processor.compute_direction((double)(curr_time - last_time));
        last_time = curr_time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = in_processor.get_view_mat();
        projector = in_processor.get_proj_mat();
        P = projector;
        glUniformMatrix4fv(P_id, 1, GL_FALSE, &P[0][0]);

        V = view;
        glUniformMatrix4fv(V_id, 1, GL_FALSE, &V[0][0]);

        glm::vec3 light_position_cameraspace(view * glm::vec4(light_position_worldspace, 1.0));
        glUniform3f(light_pos_id, light_position_cameraspace.x, light_position_cameraspace.y, light_position_cameraspace.z);


        glDrawElementsInstanced(GL_TRIANGLES, element_array.size(), GL_UNSIGNED_INT, 0, instance_displacements.size());

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
        swap_sdl_gl_window();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                carry_on = false;
            }
            in_processor.process_event(&event);
        }
    }

    destroy_sdl_gl_window();
}
