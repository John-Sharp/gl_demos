#ifndef ENGINE_H
#define ENGINE_H

#include "engine.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

engine eng;

void setup_sprite_vertex_data();
void setup_shader();
void setup_projection_matrix();

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
    eng.w = w;
    eng.h = h;

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

    setup_sprite_vertex_data();

    setup_shader();

    setup_projection_matrix();

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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        SDL_GL_SwapWindow(eng.window);
    }
}

void setup_sprite_vertex_data()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f,  1.0f,
        -1.0f,  1.0f,
        1.0f, -1.0f,
        1.0f,  1.0f
    } ;

    glGenBuffers(1, &(eng.sprite_vertex_bo));
    glBindBuffer(GL_ARRAY_BUFFER, eng.sprite_vertex_bo);
    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(vertices),
            vertices,
            GL_STATIC_DRAW);
}

void setup_shader()
{
    char simple_vertexshader[] = {
#include "simple_vertexshader.h"
    };
    const char * p_simple_vertexshader = simple_vertexshader;

    char simple_fragmentshader[] = {
#include "simple_fragmentshader.h"
    };
    const char * p_simple_fragmentshader = simple_fragmentshader;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &(p_simple_vertexshader), NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &(p_simple_fragmentshader), NULL);
    glCompileShader(vertex_shader);

    eng.shader_program = glCreateProgram();
    glAttachShader(eng.shader_program, vertex_shader);
    glAttachShader(eng.shader_program, fragment_shader);
    glLinkProgram(eng.shader_program);
    glUseProgram(eng.shader_program);

    glBindBuffer(GL_ARRAY_BUFFER, eng.sprite_vertex_bo);
    GLint pos_attrib = glGetAttribLocation(eng.shader_program, "pos"); 
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(pos_attrib);
}

void setup_projection_matrix()
{
    GLfloat P[] = {
        1/(float)eng.w, 0,              0, 0,
        0,              1/(float)eng.h, 0, 0,
        0,              0,              0, 0,
        0,              0,              0, 1 
    };

    GLint P_unfm = glGetUniformLocation(eng.shader_program, "P");
    glUniformMatrix4fv(P_unfm, 1, GL_FALSE, P);
}
#endif
