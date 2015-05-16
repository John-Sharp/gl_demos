#include <vector>
#include <stdio.h>

#include <GL/gl.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "bin_obj_reader.hpp"

void bin_obj_read(
    const char *path,
    std::vector<GLuint> &element_array,
    std::vector<glm::vec3> &vertices,
    std::vector<glm::vec2> &uvs,
    std::vector<glm::vec3> &normals,
    std::vector<glm::vec3> *ordered_vertices,
    std::vector<GLuint> *element_array_vertices_ordered_vertices_map
) {
    FILE *fp = fopen(path, "r");
    int element_array_size, vertices_size, ordered_vertices_size;

    if (ordered_vertices) {
        fscanf(
            fp,
            "%d %d %d\n",
            &element_array_size,
            &vertices_size,
            &ordered_vertices_size);
    } else {
        fscanf(fp, "%d %d\n", &element_array_size, &vertices_size);
    }

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

    if (ordered_vertices) {
        ordered_vertices->resize(ordered_vertices_size);
        fread(
            &((*ordered_vertices)[0]),
            sizeof((*ordered_vertices)[0]),
            ordered_vertices_size,
            fp);

        element_array_vertices_ordered_vertices_map->resize(vertices_size);
        fread(
            &((*element_array_vertices_ordered_vertices_map)[0]),
            sizeof((*element_array_vertices_ordered_vertices_map)[0]),
            element_array_size,
            fp);
    }

    fclose(fp);
}
