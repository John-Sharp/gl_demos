#include <vector>
#include <stdio.h>

#include <GL/gl.h>
#include <glm/glm.hpp>

#include "bin_obj_reader.hpp"

void bin_obj_read(
    const char *path,
    std::vector<GLuint> &element_array,
    std::vector<glm::vec3> &vertices,
    std::vector<glm::vec2> &uvs,
    std::vector<glm::vec3> &normals
) {
    FILE *fp = fopen(path, "r");
    int element_array_size, vertices_size;

    fscanf(fp, "%d %d\n", &element_array_size, &vertices_size);

    element_array.resize(element_array_size);
    fread(
        &element_array[0],
        sizeof(element_array[0]),
        element_array_size,
        fp);

    vertices.resize(vertices_size);
    fread(
        &vertices[0],
        sizeof(vertices[0]),
        vertices_size,
        fp);

    uvs.resize(vertices_size);
    fread(
        &uvs[0],
        sizeof(uvs[0]),
        vertices_size,
        fp);

    normals.resize(vertices_size);
    fread(
        &normals[0],
        sizeof(normals[0]),
        vertices_size,
        fp);
}
