#include "snake_actor.h"

void snake_render_handler(actor *a)
{
    snake_actor *sn = (snake_actor *)a;

    int i;
    for (i = 0; i < sn->num_segments; i++) {
        sprite_render(&(sn->snake_sprites[i]));
    }
}

snake_actor *snake_actor_init(snake_actor *sn)
{
    sn->num_segments = 3;
    actor_init(&(sn->a), snake_render_handler);
    int i;
    enum SPRITES_DECALS snake_init_config[] = {TAIL_E, BODY_E, HEAD_E};
    GLfloat tail_pos[] = {40, 40};
    for (i = 0; i < sn->num_segments; i++) {
        sprite_init(
                &(sn->snake_sprites[i]),
                40,
                40,
                &(eng.sprites_decals[snake_init_config[i]]));
        sn->snake_sprites[i].r[0] = tail_pos[0] + i * 40;
        sn->snake_sprites[i].r[1] = tail_pos[1];
    }
    return sn;
}
