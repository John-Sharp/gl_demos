#ifndef APPLE_ACTOR_H
#define APPLE_ACTOR_H

typedef struct apple_actor apple_actor;

struct apple_actor {
    actor a;
    sprite sprite;
};

apple_actor *apple_actor_init(apple_actor *ap);

void apple_replace(apple_actor *ap);
#endif
