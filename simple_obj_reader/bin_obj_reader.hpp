#ifndef BIN_OBJ_READER_H
#define BIN_OBJ_READER_h
#include <vector>

#include <GL/gl.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

void bin_obj_read(
    const char *path,
    std::vector<GLuint> &element_array,
    std::vector<glm::vec3> &vertices,
    std::vector<glm::vec2> &uvs,
    std::vector<glm::vec3> &normals,
    std::vector<glm::vec3> *ordered_vertices = NULL,
    std::vector<GLuint> *element_array_vertices_ordered_vertices_map = NULL
);
#endif
