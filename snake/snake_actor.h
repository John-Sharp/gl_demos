#ifndef SNAKE_ACTOR_H
#define SNAKE_ACTOR_H

#include "engine.h"

typedef struct snake_actor snake_actor;

struct snake_actor {
    actor a;
    int num_segments;
    sprite snake_sprites[MAX_SNAKE_SEGMENTS];
};

snake_actor *snake_actor_init(snake_actor *sn);

#endif
