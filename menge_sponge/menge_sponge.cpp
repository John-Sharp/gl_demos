#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdlib.h>
#include <stdio.h>
#include "../simple_obj_reader/bin_obj_reader.hpp"
#include "../shader_compiler/shader_compiler.hpp"
#include "../InputProcessor/InputProcessor.hpp"

SDL_GLContext main_context;
SDL_Window *window;

void init_sdl_gl_window(int w, int h, const char*title)
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Video initialisation failed: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            w,
            h,
            SDL_WINDOW_OPENGL);

    if(window == NULL) {
        fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
        exit(1);
    }

    main_context = SDL_GL_CreateContext(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(1);
    }

    SDL_GL_SetSwapInterval(1);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS); 
}

void destroy_sdl_gl_window()
{
    SDL_GL_DeleteContext(main_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() 
{
    SDL_Event event;
    init_sdl_gl_window(800, 600, "Menge Sponge");
    bool carry_on = true;
    std::vector<GLuint> element_array;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    bin_obj_read(
        "obj", 
        element_array,
        vertices,
        uvs,
        normals);

    GLuint shader_program = compile_shader(
        "../shaders/simple.vertexshader",
        "../shaders/simple.fragmentshader");
    glUseProgram(shader_program);

    GLuint element_bo;
    glGenBuffers(1, &element_bo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_bo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        element_array.size() * sizeof(element_array[0]),
        &element_array[0],
        GL_STATIC_DRAW);

    GLuint vertex_bo;//, uv_bo, normal_bo;

    glGenBuffers(1, &vertex_bo);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_bo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(vertices[0]),
        &vertices[0],
        GL_STATIC_DRAW);

    GLint pos_attrib = glGetAttribLocation(shader_program, "position");
    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(pos_attrib);

    // Get handle for our "MVP" uniform
    GLuint MVP_id = glGetUniformLocation(shader_program, "MVP");

    InputProcessor in_processor(0.001, 0.001, glm::vec3(0.0, 0.0, 4));

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = in_processor.get_view_mat();
    glm::mat4 projector = in_processor.get_proj_mat();
    glm::mat4 MVP = projector * view * model;

    glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &MVP[0][0]);

    Uint32 curr_time = SDL_GetTicks();
    Uint32 last_time = SDL_GetTicks();
    while(carry_on) {
        curr_time = SDL_GetTicks();
        in_processor.compute_direction((double)(curr_time - last_time));
        last_time = curr_time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = in_processor.get_view_mat();
        projector = in_processor.get_proj_mat();
        MVP = projector * view * model;

        glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &MVP[0][0]);

        glDrawElements(GL_TRIANGLES, element_array.size(), GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(window);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                carry_on = false;
            }
            in_processor.process_event(&event);
        }
    }

    destroy_sdl_gl_window();
}
