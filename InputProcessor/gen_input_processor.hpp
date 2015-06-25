#ifndef GEN_INPUT_PROCESSOR_H
#define GEN_INPUT_PROCESSOR_H

#include <SDL2/SDL.h>
#include <set>
#include <algorithm>

enum binding_type {
    BINDING_ATOMIC,      // press button to turn state on,
                         // press again to switch off

    BINDING_CONTINUOUS   // press button and state turns on,
                         // release and state turns off
};
template <class states_enum>
class binding {
    public:
        SDL_Keycode k;
        states_enum s;
        binding_type t;

        friend bool operator== (const binding &a, SDL_Keycode ck) {
            if (a.k == ck) {
                return true;
            }
            return false;
        }

        friend bool operator< (const binding &a, const binding &b)
        {
            if (a.k == b.k && a.s == b.s) {
                return false;
            }
    
            if (a.k <= b.k) {
                return true;
            }

            return false;
        }
};

class BaseInputProcessor {
};

template <class states_enum> class GenInputProcessor : public BaseInputProcessor {
    public:
        void test_meth(states_enum s);
        void add_key_binding(SDL_Keycode k, states_enum s, binding_type t);
        void rm_key_binding(SDL_Keycode k, states_enum s, binding_type t);
        void process_input(SDL_Event *event);
        bool is_state_active(states_enum s);
        void activate_state(states_enum s);
        void deactivate_state(states_enum s);

    private:
        std::set<binding<states_enum> > bindings;
        std::set<states_enum> active_states;
};

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
    states_enum s,
    binding_type t)
{
    binding<states_enum> b = {.k = k, .s = s, .t = t};
    bindings.insert(b);
}
       
template <class states_enum>
void GenInputProcessor<states_enum>::rm_key_binding(
    SDL_Keycode k,
    states_enum s,
    binding_type t)
{
    binding<states_enum> b = {.k = k, .s = s, .t = t};
    bindings.erase(b);
}

#include <iostream>
template <class states_enum>
void GenInputProcessor<states_enum>::process_input(SDL_Event *event)
{
    SDL_Keycode key = event->key.keysym.sym;

    typename std::set<binding<states_enum> >::iterator res = \
        std::find(bindings.begin(), bindings.end(), key);

    if (res != bindings.end()) {
        if (event->key.type == SDL_KEYDOWN) {
            if (res->t == BINDING_ATOMIC && is_state_active(res->s)) {
                active_states.erase(res->s);
                return;
            }
            active_states.insert(res->s);
        } else if (res->t == BINDING_CONTINUOUS) {
            active_states.erase(res->s);
        }
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

template <class states_enum>
void GenInputProcessor<states_enum>::activate_state(states_enum s)
{
    active_states.insert(s);
}

template <class states_enum>
void GenInputProcessor<states_enum>::deactivate_state(states_enum s)
{
    active_states.erase(s);
}

#endif
