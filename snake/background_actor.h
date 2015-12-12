#ifndef BACKGROUND_ACTOR_H
#define BACKGROUND_ACTOR_H
#include "engine.h"

typedef struct background_actor background_actor;

struct background_actor {
    actor a;
    sprite background_sprite;
};

background_actor *background_actor_init(background_actor *bg);
#endif
