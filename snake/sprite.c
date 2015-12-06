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
        GLuint texture_index,
        const SDL_Rect *texture_area)
{
    sp->w = w;
    sp->h = h;
    sp->texture_index = texture_index;

    GLfloat uvs_tmp[] = {
        0.0f, 1.0f, // bottom left
        1.0f, 1.0f, // bottom right
        0.0f, 0.0f, // top left
        0.0f, 0.0f, // top left
        1.0f, 1.0f, // bottom right
        1.0f, 0.0f  // top right
    };

    memcpy(sp->uvs, uvs_tmp, 12 * sizeof(uvs_tmp[0]));

    glGenBuffers(1, &sp->uv_bo);
    glBindBuffer(GL_ARRAY_BUFFER, sp->uv_bo);
    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(sp->uvs),
            sp->uvs,
            GL_STATIC_DRAW);

    glGenVertexArrays(1, &sp->vao);

    glBindVertexArray(sp->vao);

    glBindBuffer(GL_ARRAY_BUFFER, sp->uv_bo);

    glVertexAttribPointer(eng.uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(eng.uv_attrib);

    glBindBuffer(GL_ARRAY_BUFFER, eng.sprite_vertex_bo);
    glVertexAttribPointer(eng.pos_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(eng.pos_attrib);

    return sp;
}

void sprite_set_uvs(sprite *sp, const GLfloat *uvs)
{
    glBindVertexArray(sp->vao);
    memcpy(sp->uvs, uvs, 12 * sizeof(uvs[0]));
    glBindBuffer(GL_ARRAY_BUFFER, sp->uv_bo);
    glBufferData(
            GL_ARRAY_BUFFER,
            12 * sizeof(sp->uvs[0]),
            sp->uvs,
            GL_STATIC_DRAW);
}

void sprite_render(sprite *sp)
{
    glBindVertexArray(sp->vao);
    glUniform1i(eng.texture_sampler_unfm, sp->texture_index);
    glUniform1f(eng.w_unfm, sp->w);
    glUniform1f(eng.h_unfm, sp->h);
    glUniform2fv(eng.r_unfm, 1, sp->r);
    glDrawArrays(GL_TRIANGLES, 0, 6);

        GLenum errno;
        while ((errno = glGetError())) {
            fprintf(stderr, "%x\n", errno);
        }
}
