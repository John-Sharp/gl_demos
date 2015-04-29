#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <GL/gl.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

struct face {
    int v;
    int t;
    int n;

    bool operator==(struct face f2)
    {
        if (memcmp(this, &f2, sizeof(*this)) == 0) {
            return true;
        }
        return false;
    }
};

void process_face(
        struct face new_face,
        std::vector<GLuint> &element_array,
        std::vector<struct face> &temp_faces
) {
    std::vector<struct face>::iterator it;

    it = find(
            temp_faces.begin(),
            temp_faces.end(),
            new_face);
    if (it != temp_faces.end()) {
        element_array.push_back(it - temp_faces.begin());
        return;
    }

    element_array.push_back(temp_faces.size());
    temp_faces.push_back(new_face);
}

void simple_obj_read(
        const char *path,
        std::vector<GLuint> &element_array,
        std::vector<glm::vec3> &vertices,
        std::vector<glm::vec2> &uvs,
        std::vector<glm::vec3> &normals
) {
    FILE *file = fopen(path, "r");
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    std::vector<struct face> temp_faces;

    if (file == NULL) {
        fprintf(stderr, "File couldn't be opened\n");
        exit(1);
    }

    char line_buffer[128];

    while ((fgets(line_buffer, sizeof(line_buffer) - 1, file) != NULL)) {
        char entry_type[128];
        sscanf(line_buffer, "%s", entry_type);

        if (strcmp(entry_type, "v") == 0) {
            glm::vec3 vertex;
            sscanf(
                &(line_buffer[1]),
                "%f %f %f\n",
                &vertex.x,
                &vertex.y,
                &vertex.z);
            temp_vertices.push_back(vertex);
            continue;
        }

        if (strcmp(entry_type, "vt") == 0) {
            glm::vec2 uv;
            sscanf(
                &(line_buffer[2]),
                "%f %f\n",
                &uv.x,
                &uv.y);
            temp_uvs.push_back(uv);
            continue;
        }

        if (strcmp(entry_type, "vn") == 0) {
            glm::vec3 normal;
            sscanf(
                &(line_buffer[2]),
                "%f %f %f\n",
                &normal.x,
                &normal.y,
                &normal.z);
            temp_normals.push_back(normal);
            continue;
        }

        if (strcmp(entry_type, "f") == 0) {
            struct face triangle_points[3];

            sscanf(
                &(line_buffer[1]),
                "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &triangle_points[0].v,
                &triangle_points[0].t,
                &triangle_points[0].n,
                &triangle_points[1].v,
                &triangle_points[1].t,
                &triangle_points[1].n,
                &triangle_points[2].v,
                &triangle_points[2].t,
                &triangle_points[2].n);

            for (int i = 0; i < 3; i++) {
                process_face(triangle_points[i], element_array, temp_faces);
            }
        }
    }

    std::vector<struct face>::iterator it;

    for (it = temp_faces.begin(); it != temp_faces.end(); it++) {
        // fprintf(stderr, "%d %d %d\n", it->v, it->t, it->n);
        vertices.push_back(temp_vertices[it->v - 1]);
        uvs.push_back(temp_uvs[it->t - 1]);
        normals.push_back(temp_normals[it->n - 1]);
    }

    return;
}

void simple_obj_write(
        std::vector<GLuint> &element_array,
        std::vector<glm::vec3> &vertices,
        std::vector<glm::vec2> &uvs,
        std::vector<glm::vec3> &normals
) {
    fprintf(stdout, "%lu %lu\n", element_array.size(), vertices.size());
    fwrite(
        &element_array[0],
        sizeof(element_array[0]),
        element_array.size(),
        stdout);
    fwrite(
        &vertices[0],
        sizeof(vertices[0]),
        vertices.size(),
        stdout);
    fwrite(
        &uvs[0],
        sizeof(uvs[0]),
        uvs.size(),
        stdout);
    fwrite(
        &normals[0],
        sizeof(normals[0]),
        normals.size(),
        stdout);
}


int main(int argc, char *argv[]) 
{
    std::vector<GLuint> element_array;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    simple_obj_read(
            argv[1],
            element_array,
            vertices,
            uvs,
            normals);
    simple_obj_write(
            element_array,
            vertices,
            uvs,
            normals);

}
