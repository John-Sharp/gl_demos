#ifndef GEN_INPUT_PROCESSOR_H
#define GEN_INPUT_PROCESSOR_H

#include <SDL2/SDL.h>
#include <set>
#include <algorithm>

template <class states_enum>
class binding {
    public:
        SDL_Keycode k;
        states_enum s;

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
        void add_key_binding(SDL_Keycode k, states_enum s);
        void rm_key_binding(SDL_Keycode k, states_enum s);
        void process_input(SDL_Event *event);
        bool is_state_active(states_enum s);

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
        std::find(bindings.begin(), bindings.end(), key);

    if (res != bindings.end()) {
        if (event->key.type == SDL_KEYDOWN) {
            active_states.insert(res->s);
        } else {
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

#endif
