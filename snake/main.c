#include "engine.h"

int main()
{
    engine * eng = engine_init(
            800,
            600,
            "Snake",
            100);

   engine_start();

    engine_destroy();
}
