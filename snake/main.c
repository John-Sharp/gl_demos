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
    sprite_init(&sp, w, h, SPRITES_TEXTURE, NULL);
    sp.r[0] = 25;
    sp.r[1] = 25;

    GLfloat uvs_tmp[] = {
        7 * 0.125f, 1.0f, // bottom left
        8 * 0.125f, 1.0f, // bottom right
        7 * 0.125f, 0.501f, // top left
        7 * 0.125f, 0.501f, // top left
        8 * 0.125f, 1.0f, // bottom right
        8 * 0.125f, 0.501f  // top right
    };
    sprite_set_uvs(&sp, uvs_tmp);

    eng->render_list = sprite_list_add(eng->render_list, &sp);

    sprite sp2;
    sprite_init(&sp2, 800, 600, SAND_TEXTURE, NULL);
    sp2.r[0] = 400;
    sp2.r[1] = 300;
    eng->render_list = sprite_list_add(eng->render_list, &sp2);

    engine_start();

    engine_destroy();
}
