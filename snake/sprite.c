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
        decal *d)
{
    sp->w = w;
    sp->h = h;
    sp->d = d;

    glGenVertexArrays(1, &sp->vao);

    glBindVertexArray(sp->vao);

    glBindBuffer(GL_ARRAY_BUFFER, d->uv_bo);

    glVertexAttribPointer(eng.uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(eng.uv_attrib);

    glBindBuffer(GL_ARRAY_BUFFER, eng.sprite_vertex_bo);
    glVertexAttribPointer(eng.pos_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(eng.pos_attrib);

    return sp;
}

void sprite_render(sprite *sp)
{
    glBindVertexArray(sp->vao);
    glUniform1i(eng.texture_sampler_unfm, sp->d->texture_index);
    glUniform1f(eng.w_unfm, sp->w);
    glUniform1f(eng.h_unfm, sp->h);
    glUniform2fv(eng.r_unfm, 1, sp->r);
    glDrawArrays(GL_TRIANGLES, 0, 6);


        GLenum errno;
        while ((errno = glGetError())) {
            fprintf(stderr, "%x\n", errno);
        }
}
