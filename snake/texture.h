#ifndef TEXTURE_H
#define TEXTURE_H

struct texture
{
    GLuint texture_id;
}

texture *texture_init(
        texture *txt,
        const char *fname);
#endif
