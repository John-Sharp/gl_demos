#include "background_actor.h"

void background_render_handler(actor *a)
{
    background_actor *bg = (background_actor *)a;

    sprite_render(&(bg->background_sprite));
}

background_actor *background_actor_init(background_actor *bg)
{
    actor_init(&bg->a, background_render_handler);
    sprite_init(
            &bg->background_sprite, 
            800,
            600,
            &eng.sand_decal);
    bg->background_sprite.r[0] = 400;
    bg->background_sprite.r[1] = 300;
    return bg;
}
