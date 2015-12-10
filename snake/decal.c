#include "engine.h"
#include "decal.h"

decal *decal_init(
        decal *d,
        GLuint texture_index,
        GLfloat x,
        GLfloat y,
        GLfloat w,
        GLfloat h
)
{
    d->texture_index = texture_index;

    glGenBuffers(1, &d->uv_bo);
    decal_set_xywh(d, x, y, w, h);
    return d;
}

void decal_set_xywh( decal *d,
        GLfloat x,
        GLfloat y,
        GLfloat w,
        GLfloat h)
{
    GLfloat uvs_tmp[] = {
        x, y + h, // bottom left
        x + w, y + h, // bottom right
        x, y, // top left
        x, y, // top left
        x + w, y + h, // bottom right
        x + w, y  // top right
    };

    memcpy(d->uvs, uvs_tmp, 12 * sizeof(uvs_tmp[0]));

    glBindBuffer(GL_ARRAY_BUFFER, d->uv_bo);

    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(d->uvs),
            d->uvs,
            GL_STATIC_DRAW);
}
