#ifndef BIN_OBJ_READER_H
#define BIN_OBJ_READER_h
#include <vector>

#include <GL/gl.h>
#include <glm/glm.hpp>

void bin_obj_read(
    const char *path,
    std::vector<GLuint> &element_array,
    std::vector<glm::vec3> &vertices,
    std::vector<glm::vec2> &uvs,
    std::vector<glm::vec3> &normals
);
#endif
