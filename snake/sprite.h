#ifndef SPRITE_H
#define SPRITE_H

typedef struct sprite sprite;

struct sprite
{
    GLuint vertex_bo;
};

sprite *sprite_init(
        sprite *sp,
        double w,
        double h,
        texture *tex,
        SDL_Rect texture_area);

void sprite_destroy(sprite *sp);

void sprite_render(sprite *sp);

#endif
