#define DEBUG
#include "engine.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

void setup_sprite_vertex_data();
void setup_shader();
void setup_projection_matrix();
void setup_uniforms();
void setup_attributes();
void setup_texture_params();
void setup_textures();
void setup_decals();
void setup_bindings();
bool should_continue_logic_loops();

engine *engine_init(
        unsigned int w,
        unsigned int h,
        const char *window_title,
        unsigned int fps)
{
    eng.fps = fps;
    eng.current_frame = 0;

    eng.start_time = 0;
    eng.should_start_logic_loop = true;
    eng.whole_frames_to_do = 0;

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

    setup_uniforms();
    setup_attributes();

    setup_textures();
    setup_decals();

    setup_bindings();

    eng.render_list = NULL;
    eng.logic_list = NULL;

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

    eng.start_time = SDL_GetTicks();
    while (carry_on) {
        process_input();

        if (is_state_active(GS_QUIT)) {
            carry_on = false;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        actor_list *al;
        for (al = eng.render_list; al != NULL; al = al->next) {
            al->a->render_handler(al->a);
        }

        SDL_GL_SwapWindow(eng.window);

        while (should_continue_logic_loops()) {
            for (al = eng.logic_list; al != NULL; al = al->next) {
                al->a->logic_handler(al->a);
            }
        }
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
    

#ifdef DEBUG
    GLint status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    char buffer[512];
    glGetShaderInfoLog(vertex_shader, 512, NULL, buffer);
    fprintf(stderr, "%s\n", buffer);

    glGetProgramInfoLog(eng.shader_program, 512, NULL, buffer);
    fprintf(stderr, "%s\n", buffer);
#endif

    glBindBuffer(GL_ARRAY_BUFFER, eng.sprite_vertex_bo);
    GLint pos_attrib = glGetAttribLocation(eng.shader_program, "pos"); 
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(pos_attrib);
}

void setup_projection_matrix()
{
    GLfloat P[] = {
        2/(float)eng.w, 0,              0, -1,
        0,              2/(float)eng.h, 0, -1,
        0,              0,              0,  0,
        0,              0,              0,  1 
    };

    GLint P_unfm = glGetUniformLocation(eng.shader_program, "P");
    glUniformMatrix4fv(P_unfm, 1, GL_TRUE, P);
}

void setup_uniforms()
{
    eng.w_unfm = glGetUniformLocation(eng.shader_program, "w");
    eng.h_unfm = glGetUniformLocation(eng.shader_program, "h");
    eng.r_unfm = glGetUniformLocation(eng.shader_program, "r");
    eng.texture_sampler_unfm = glGetUniformLocation(
            eng.shader_program, "texture_sampler");
}

void setup_attributes()
{
    eng.pos_attrib = glGetAttribLocation(eng.shader_program, "pos"); 
    eng.uv_attrib = glGetAttribLocation(eng.shader_program, "uv"); 
}

void setup_texture_params()
{
    // Possible values: 
    //  GL_REPEAT
    //  GL_MIRRORED_REPEAT
    //  GL_CLAMP_TO_EDGE
    //  GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
}

void setup_textures()
{
    char * filenames[] = {
        "img/sand.png",
        "img/sprites.png"
    };
    int i;

    glGenTextures(NUM_TEXTURES, eng.textures);

    for (i = 0; i < sizeof(filenames) / sizeof(filenames[0]); i++) {
        char * fname = filenames[i];
        SDL_Surface *img = IMG_Load(fname);

        if(!img){
            fprintf(stderr, "Error! Could not load %s\n", fname);
            exit(1);
        }
        
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, eng.textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h,
                0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
        SDL_FreeSurface(img);

        setup_texture_params();
    }
}

void setup_decals()
{
    int i;

    for (i = 0; i < NUM_SPRITES_DECALS; i++) {
        GLfloat w = 1/8.;
        GLfloat h = 1/2.;
        GLfloat x = (float)(i % 8) * w;
        GLfloat y = (float)(i / 8) * h;

        decal_init(
                &eng.sprites_decals[i],
                SPRITES_TEXTURE,
                x,
                y,
                w,
                h);
    }

    decal_init(
        &(eng.sand_decal),
        SAND_TEXTURE,
        0,
        0,
        1,
        1);
}

bool should_continue_logic_loops()
{
    if (eng.should_start_logic_loop) {
        unsigned int logic_loop_start_time = SDL_GetTicks();
        double elapsed_frames = (double)(logic_loop_start_time \
                - eng.start_time) / 1000.0f * eng.fps;
        eng.whole_frames_to_do = (unsigned int)elapsed_frames - eng.current_frame;
    }

    if (!eng.whole_frames_to_do) {
        eng.should_start_logic_loop = true;
        return false;
    }

    eng.whole_frames_to_do -= 1;
    eng.current_frame += 1;
    eng.should_start_logic_loop = false;
    return true;
}

void setup_bindings()
{
    input_processor_init();
    key_state_binding binding;

    binding.k = SDLK_UP;
    binding.s = GS_N;
    binding.t = BINDING_ONE_TIME;
    add_binding(&binding);

    binding.k = SDLK_RIGHT;
    binding.s = GS_E;
    add_binding(&binding);

    binding.k = SDLK_DOWN;
    binding.s = GS_S;
    add_binding(&binding);

    binding.k = SDLK_LEFT;
    binding.s = GS_W;
    add_binding(&binding);
}
