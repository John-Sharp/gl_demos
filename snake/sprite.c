#include "sprite.h"

sprite *sprite_init(
        sprite *sp,
        double w,
        double h,
        texture *tex,
        SDL_Rect texture_area)
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f
    } ;
        
    glBindVertexArray(sp->vao);

    glGenBuffers(1, &(sp->vertex_bo));
    glBindBuffer(GL_ARRAY_BUFFER, sp->vertex_bo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);

    glBindVertexArray(sp->vao);

    return sp;
}
