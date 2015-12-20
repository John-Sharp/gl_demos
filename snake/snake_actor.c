#include "snake_actor.h"

void snake_render_handler(actor *a)
{
    snake_actor *sn = (snake_actor *)a;

    int i;
    for (i = 0; i < sn->num_segments; i++) {
        sprite_render(&(sn->snake_sprites[i]));
    }
}

void direction_to_vector(DIRECTION direction, GLfloat *r)
{
    switch (direction) {
        case DIRECTION_N:
            r[0] = 0;
            r[1] = 40;
            break;
        case DIRECTION_E:
            r[0] = 40;
            r[1] = 0;
            break;
        case DIRECTION_S:
            r[0] = 0;
            r[1] = -40;
            break;
        case DIRECTION_W:
            r[0] = -40;
            r[1] = 0;
            break;
    }
}

SPRITES_DECALS snake_get_neck_decal(snake_actor *sn, DIRECTION old_direction)
{
    switch (sn->direction) {
        case DIRECTION_N:
            switch (old_direction) {
                case DIRECTION_W:
                    return BODY_NE;
                case DIRECTION_E:
                    return BODY_NW;
                default:
                    return BODY_N;
            }
        case DIRECTION_E:
            switch (old_direction) {
                case DIRECTION_N:
                    return BODY_SE;
                case DIRECTION_S:
                    return BODY_NE;
                default:
                    return BODY_E;
            }
        case DIRECTION_S:
            switch (old_direction) {
                case DIRECTION_W:
                    return BODY_SE;
                case DIRECTION_E:
                    return BODY_SW;
                default:
                    return BODY_N;
            }
            return BODY_N;
        case DIRECTION_W:
            switch (old_direction) {
                case DIRECTION_N:
                    return BODY_SW;
                case DIRECTION_S:
                    return BODY_NW;
                default:
                    return BODY_E;
            }
    }
    return BODY_N;
}

SPRITES_DECALS snake_get_head_decal(snake_actor *sn)
{
    switch (sn->direction) {
        case DIRECTION_N:
            return HEAD_N;
        case DIRECTION_E:
            return HEAD_E;
        case DIRECTION_S:
            return HEAD_S;
        case DIRECTION_W:
            return HEAD_W;
    }
    return HEAD_N;
}

SPRITES_DECALS snake_get_tail_decal(snake_actor *sn)
{
    unsigned int next_to_tail_index = (sn->tail_index + 1) % sn->num_segments;

    if (sn->snake_sprites[next_to_tail_index].r[0] > sn->snake_sprites[sn->tail_index].r[0]) {
        return TAIL_E;
    }
    
    if (sn->snake_sprites[next_to_tail_index].r[0] < sn->snake_sprites[sn->tail_index].r[0]) {
        return TAIL_W;
    }

    if (sn->snake_sprites[next_to_tail_index].r[1] > sn->snake_sprites[sn->tail_index].r[1]) {
        return TAIL_N;
    }

    return TAIL_S;
}

void snake_logic_handler(actor *a)
{
    snake_actor * sn = (snake_actor *)a;
    if (eng.current_frame % sn->snake_frame_ratio == 0) {
        DIRECTION old_direction = sn->direction;

        if (is_state_active(GS_N)) {
            deactivate_state(GS_N);
            if (sn->direction != DIRECTION_S)
            {
                sn->direction = DIRECTION_N;
            }
        } else if (is_state_active(GS_E)) {
            deactivate_state(GS_E);
            if (sn->direction != DIRECTION_W)
            {
                sn->direction = DIRECTION_E;
            }
        } else if (is_state_active(GS_S)) {
            deactivate_state(GS_S);
            if (sn->direction != DIRECTION_N)
            {
                sn->direction = DIRECTION_S;
            }
        } else if (is_state_active(GS_W)) {
            deactivate_state(GS_W);

            if (sn->direction != DIRECTION_E)
            {
                sn->direction = DIRECTION_W;
            }
        }

        int old_head_index = ((sn->tail_index - 1 + sn->num_segments) % sn->num_segments);
        sprite *old_head = &sn->snake_sprites[old_head_index];
        SPRITES_DECALS decal = snake_get_neck_decal(sn, old_direction);
        sprite_set_decal(old_head, &(eng.sprites_decals[decal]));


        sprite *new_head = &sn->snake_sprites[sn->tail_index];
        direction_to_vector(sn->direction, new_head->r);
        new_head->r[0] += old_head->r[0];
        new_head->r[1] += old_head->r[1];

        decal = snake_get_head_decal(sn);
        sprite_set_decal(new_head, &(eng.sprites_decals[decal]));
        
        sn->tail_index = (sn->tail_index + 1) % sn->num_segments;

        decal = snake_get_tail_decal(sn);
        sprite_set_decal(&sn->snake_sprites[sn->tail_index], &(eng.sprites_decals[decal]));
    }
}

snake_actor *snake_actor_init(snake_actor *sn)
{
    sn->num_segments = 3;
    actor_init(&(sn->a), snake_render_handler, snake_logic_handler);
    int i;
    SPRITES_DECALS snake_init_config[] = {TAIL_E, BODY_E, HEAD_E};
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
    sn->direction = DIRECTION_E;

    float seconds_per_update = 0.1;
    sn->snake_frame_ratio = eng.fps * seconds_per_update;

    sn->tail_index = 0;
    return sn;
}
