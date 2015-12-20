#ifndef SPRITE_H
#define SPRITE_H

#include "decal.h"

typedef struct sprite sprite;
typedef struct sprite_list sprite_list;

struct sprite
{
    GLfloat w;
    GLfloat h;
    GLfloat r[2];

    GLuint vao;

    decal *d;
};

sprite *sprite_init(
        sprite *sp,
        double w,
        double h,
        decal *d);

void sprite_destroy(sprite *sp);

void sprite_render(sprite *sp);

void sprite_set_decal(sprite *sp, decal *d);

#endif
