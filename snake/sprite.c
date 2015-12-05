#include "engine.h"
#include "sprite.h"

sprite_list *sprite_list_add(sprite_list *spl, sprite *sp)
{
    sprite_list *new_node = malloc(sizeof(*new_node));
    new_node->sp = sp;
    new_node->next = spl;
    return new_node;
}

sprite *sprite_init(
        sprite *sp,
        double w,
        double h,
        int *tex,
        const SDL_Rect *texture_area)
{
    sp->w = w;
    sp->h = h;
    return sp;
}

void sprite_render(sprite *sp)
{
    glUniform1f(eng.w_unfm, sp->w);
    glUniform1f(eng.h_unfm, sp->h);
    glUniform2fv(eng.r_unfm, 1, sp->r);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
