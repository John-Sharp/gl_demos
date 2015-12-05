#ifndef SPRITE_H
#define SPRITE_H

typedef struct sprite sprite;
typedef struct sprite_list sprite_list;

struct sprite
{
    GLfloat w;
    GLfloat h;
    GLfloat r[2];
};

struct sprite_list
{
    sprite *sp;
    sprite_list *next;
};

sprite_list *sprite_list_add(sprite_list *spl, sprite *sp);

sprite *sprite_init(
        sprite *sp,
        double w,
        double h,
        int *tex,
        const SDL_Rect *texture_area);

void sprite_destroy(sprite *sp);

void sprite_render(sprite *sp);

#endif
