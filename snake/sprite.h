#ifndef SPRITE_H
#define SPRITE_H

typedef struct sprite sprite;
typedef struct sprite_list sprite_list;

struct sprite
{
    GLfloat w;
    GLfloat h;
    GLfloat r[2];

    GLuint uv_bo;
    GLuint vao;

    GLfloat uvs[12];

    GLuint texture_index;
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
        GLuint texture_index,
        const SDL_Rect *texture_area);

void sprite_destroy(sprite *sp);

void sprite_set_uvs(sprite *sp, const GLfloat *uvs);

void sprite_render(sprite *sp);

#endif
