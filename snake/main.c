#include "engine.h"
#include "snake_actor.h"
#include "background_actor.h"

int main()
{
    engine_init(
            800,
            600,
            "Snake",
            100);

    engine_start();

    engine_destroy();
}
