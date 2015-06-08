#include <algorithm>
#include <set>
#include <SDL2/SDL.h>
#include "gen_input_processor.hpp"

template <class states_enum>
void GenInputProcessor<states_enum>::test_meth(
    states_enum s)
{
    binding<states_enum> b = { .s = s};
    bindings.insert(b);
}
 

template <class states_enum>
void GenInputProcessor<states_enum>::add_key_binding(
    SDL_Keycode k,
    states_enum s)
{
    binding<states_enum> b = {.k = k, .s = s};
    bindings.insert(b);
}
       
template <class states_enum>
void GenInputProcessor<states_enum>::rm_key_binding(
    SDL_Keycode k,
    states_enum s)
{
    binding<states_enum> b = {.k = k, .s = s};
    bindings.erase(b);
}

template <class states_enum>
void GenInputProcessor<states_enum>::process_input(SDL_Event *event)
{
    SDL_Keycode key = event->key.keysym.sym;

    typename std::set<binding<states_enum> >::iterator res = \
        std::find_if(bindings.begin(), bindings.end(), key);

    if (res != bindings.end()) {
        active_states.insert(res->s);
    }
}

template <class states_enum>
bool GenInputProcessor<states_enum>::is_state_active(states_enum s)
{
    if (active_states.find(s) != active_states.end()) {
        return true;
    }
    return false;
}
