#include "engine.h"
#include "snake_actor.h"
#include "background_actor.h"

int main()
{
    engine *eng = engine_init(
            800,
            600,
            "Snake",
            100);

    snake_actor sn;
    snake_actor_init(&sn);
    eng->render_list = actor_list_add(eng->render_list, (actor *)(&sn));
    eng->logic_list = actor_list_add(eng->logic_list, (actor *)(&sn));

    background_actor bg;
    background_actor_init(&bg);
    eng->render_list = actor_list_add(eng->render_list, (actor *)(&bg));

    engine_start();

    engine_destroy();
}
