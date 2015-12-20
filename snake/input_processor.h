#ifndef INPUT_PROCESSOR_H
#define INPUT_PROCESSOR_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef enum game_state
{
    GS_ONE,
    GS_TWO,
    GS_N,
    GS_E,
    GS_S,
    GS_W,
    GS_QUIT
} game_state;

typedef enum binding_type
{
    BINDING_ATOMIC,      // press button to turn state on,
                         // press again to switch off

    BINDING_CONTINUOUS,  // press button and state turns on,
                         // release and state turns off

    BINDING_ONE_TIME     // press button and state turns on,
                         // state must then be turned off
                         // manually
} binding_type;

typedef struct key_state_binding key_state_binding;
struct key_state_binding
{
    SDL_Keycode k;
    game_state s; 
    binding_type t;
};


enum
{
    MAX_BINDINGS = 100
};

void input_processor_init();
bool add_binding(key_state_binding *binding);
bool rm_binding(key_state_binding *binding);
void process_input();
void activate_state(game_state state);
void deactivate_state(game_state state);
bool is_state_active(game_state state);

bool test_input_processor();

#endif
