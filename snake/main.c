#include "engine.h"

int main()
{
    engine * eng = engine_init(
            800,
            600,
            "Snake",
            100);

    sprite sp;
    GLfloat w = 50;
    GLfloat h = 50;
    sprite_init(&sp, w, h, NULL, NULL);
    sp.r[0] = 25;
    sp.r[1] = 25;
    eng->render_list = sprite_list_add(eng->render_list, &sp);

    sprite sp2;
    sprite_init(&sp2, w, h, NULL, NULL);
    sp2.r[0] = 400;
    sp2.r[1] = 300;
    eng->render_list = sprite_list_add(eng->render_list, &sp2);

    engine_start();

    engine_destroy();
}
