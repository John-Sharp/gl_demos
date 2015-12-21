#include "engine.h"
#include <stdlib.h>

void apple_render_handler(actor *a)
{
    apple_actor *ap = (apple_actor *)a;

    sprite_render(&ap->sprite);
}

apple_actor *apple_actor_init(apple_actor *ap)
{
    actor_init(&ap->a, apple_render_handler, NULL);
    sprite_init(
            &ap->sprite,
            40,
            40,
            &eng.sprites_decals[APPLE]);
    ap->sprite.r[0] = 100;
    ap->sprite.r[1] = 100;
    return ap;
}

void apple_replace(apple_actor *ap)
{
    int i = rand() % (eng.w / 40);
    int j = rand() % (eng.h / 40);
    ap->sprite.r[0] = 20 + i * 40;
    ap->sprite.r[1] = 20 + j * 40;
}
