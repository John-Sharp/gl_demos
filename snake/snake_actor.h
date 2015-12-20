#ifndef SNAKE_ACTOR_H
#define SNAKE_ACTOR_H

#include "engine.h"

typedef struct snake_actor snake_actor;

struct snake_actor {
    actor a;
    int num_segments;
    unsigned int segment_w;
    unsigned int segment_h;

    sprite snake_sprites[MAX_SNAKE_SEGMENTS];
    DIRECTION direction;


    unsigned int snake_frame_ratio;
    unsigned int tail_index;
};

snake_actor *snake_actor_init(snake_actor *sn);

#endif
