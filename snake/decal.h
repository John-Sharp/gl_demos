#ifndef DECAL_H
#define DECAL_H

typedef struct decal decal;

struct decal
{
    GLuint texture_index;
    GLuint uv_bo;
    GLfloat uvs[12];
};

decal *decal_init(
        decal *d,
        GLuint texture_index,
        GLfloat x,
        GLfloat y,
        GLfloat w,
        GLfloat h);

void decal_set_xywh(
        decal *d,
        GLfloat x,
        GLfloat y,
        GLfloat w,
        GLfloat h);

#endif
