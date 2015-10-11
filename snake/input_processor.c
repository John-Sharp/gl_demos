#include "input_processor.h"

bool active_states[GS_QUIT];
key_state_binding bindings[MAX_BINDINGS];
unsigned int number_of_bindings;

void input_processor_init()
{
    memset(active_states, 0, GS_QUIT); 
    number_of_bindings = 0;
    return;
}

int binding_key_compar(const void *v_key, const void *v_member)
{
   const SDL_Keycode *key = v_key;
   const key_state_binding *member = v_member;

   if (*key < member->k)
       return -1;
   if (*key > member->k)
       return 1;
   return 0;
}

key_state_binding *search_bindings_by_key(SDL_Keycode k)
{
    key_state_binding *b = bsearch(
            &k,
            bindings,
            number_of_bindings,
            sizeof(key_state_binding),
            binding_key_compar);

    if (b == NULL)
        return NULL;

    key_state_binding *a;

    while (b->k == k) {
        a = b;
        b -= 1;
    }

    return a;
}

bool add_binding(key_state_binding *binding)
{
    if (number_of_bindings + 1 >=  MAX_BINDINGS) {
        fprintf(stderr, "Number of bindings exceeds MAX_BINDINGS\n");
        return false;
    }

    key_state_binding *extant_binding = bindings;
    int i = 0;

    while (binding->k > extant_binding->k && i < number_of_bindings) {
        extant_binding += 1;
        i += 1;
    }

    int num_states_to_move = (number_of_bindings - i);
    
    memmove(
            &(bindings[i + 1]),
            &bindings[i],
            num_states_to_move * sizeof(key_state_binding));

    bindings[i] = *binding;
    number_of_bindings += 1;
    return true;
}

bool rm_binding(key_state_binding *binding)
{
    key_state_binding *b = search_bindings_by_key(binding->k);
    key_state_binding *b_end = 
        bindings + number_of_bindings * sizeof(key_state_binding);

    if (b == NULL)
        return true;

    while (b->k == binding->k && b != b_end) {
        if (b->s == binding->s && b->t == binding->t) {
            int i = (b - bindings) / sizeof(key_state_binding);
            int num_states_to_move = (number_of_bindings - 1 - i);

            memmove(
                    b,
                    b + 1,
                    num_states_to_move * sizeof(key_state_binding));
            number_of_bindings -= 1;
            b_end = 
                bindings + number_of_bindings * sizeof(key_state_binding);
            b--;
        }
        b++;
    }
    return true;
}

void process_event(SDL_Event *event)
{
    SDL_Keycode key = event->key.keysym.sym;

    key_state_binding *b = search_bindings_by_key(key);
    key_state_binding *b_end = 
        bindings + number_of_bindings * sizeof(key_state_binding);

    if (b == NULL)
        return;

    while (b->k == key && b != b_end) {
        if (event->key.type == SDL_KEYDOWN) {
            if (b->t == BINDING_ATOMIC && is_state_active(b->s)) {
                deactivate_state(b->s);
                b++;
                continue;
            }
            activate_state(b->s);
            b++;
            continue;
        }

        if (event->key.type == SDL_KEYUP && b->t == BINDING_CONTINUOUS)
            deactivate_state(b->s);
        b++;
    }
}

void process_input()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            active_states[GS_QUIT] = true;
        }
        process_event(&event);
    }
}

void activate_state(game_state state)
{
    active_states[state] = true;
}

void deactivate_state(game_state state)
{
    active_states[state] = false;
}

bool is_state_active(game_state state)
{
    return active_states[state];
}

bool test_add_binding()
{
    bool passed_test = true;

    key_state_binding ksb1;
    ksb1.k = SDLK_a;
    ksb1.s = GS_ONE;
    ksb1.t = BINDING_ATOMIC;

    add_binding(&ksb1);

    ksb1.k = SDLK_b;
    ksb1.s = GS_ONE;

    add_binding(&ksb1);

    ksb1.k = SDLK_a;
    ksb1.s=GS_TWO;

    add_binding(&ksb1);

    if (number_of_bindings != 3) {
        passed_test = false;
    }

    if (bindings[2].k != SDLK_b || bindings[2].s != GS_ONE) {
        passed_test = false;
    }

    if (bindings[0].k != SDLK_a || bindings[1].k != SDLK_a) {
        passed_test = false;
    }

    if (!passed_test) {
        fprintf(stderr, "add_binding test failed\n");
    }

    return passed_test;
}

bool test_rm_binding()
{
    bool passed_test = true;

    key_state_binding ksb1;
    ksb1.k = SDLK_a;
    ksb1.s = GS_TWO;
    ksb1.t = BINDING_ATOMIC;

    rm_binding(&ksb1);

    if (number_of_bindings != 2) {
        passed_test = false;
    }

    if (bindings[1].k != SDLK_b || bindings[1].s != GS_ONE) {
        passed_test = false;
    }

    rm_binding(&ksb1);

    ksb1.s = GS_ONE;
    rm_binding(&ksb1);

    if (number_of_bindings != 1) {
        passed_test = false;
    }

    if (bindings[0].k != SDLK_b || bindings[0].s != GS_ONE) {
        passed_test = false;
    }

    ksb1.k = SDLK_b;
    rm_binding(&ksb1);
    if (number_of_bindings != 0) {
        passed_test = false;
    }

    if (!passed_test) {
        fprintf(stderr, "rm_binding test failed\n");
    }

    return passed_test;
}

bool test_process_event()
{
    bool passed_test = true;
    

    key_state_binding ksb1;
    ksb1.k = SDLK_a;
    ksb1.s = GS_TWO;
    ksb1.t = BINDING_ATOMIC;
    add_binding(&ksb1);

    SDL_Event e;
    e.key.keysym.sym = SDLK_a;
    e.key.type = SDL_KEYDOWN;

    process_event(&e);

    if (!is_state_active(GS_TWO)) {
        passed_test = false;
    }

    if (!passed_test) {
        fprintf(stderr, "process_event test failed\n");
    }

    return passed_test;
}

bool test_input_processor()
{
    test_add_binding();
    test_rm_binding();
    test_process_event();
    fprintf(stderr, "tests pass\n");
    return true;
}

