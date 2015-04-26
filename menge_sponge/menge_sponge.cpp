#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdlib.h>
#include <stdio.h>
#include "../simple_obj_reader/bin_obj_reader.hpp"

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

    bin_obj_read(
            "obj", 
            element_array,
            vertices,
            uvs,
            normals);

    while(carry_on) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                carry_on = false;
            }
        }
    }

    destroy_sdl_gl_window();
}
