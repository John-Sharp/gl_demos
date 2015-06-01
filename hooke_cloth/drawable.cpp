#include <GL/glew.h>
#include <GL/gl.h>

#include "../simple_obj_reader/bin_obj_reader.hpp"
#include "../utils/utils.hpp"

#include "drawable.hpp"

void Drawable::draw()
{
    glBindVertexArray(vao);
    glUseProgram(program_id);
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    GLuint sampler_id = glGetUniformLocation(
         program_id,
         "texture_sampler");
    glUniform1i(sampler_id, texture_id - 1);

    glDrawElements(
        GL_TRIANGLES,
        element_array.size(),
        GL_UNSIGNED_INT,
        0);
}

void Drawable::update_vertices(
    std::vector<glm::vec3> &new_ordered_positions
) {
    for (
        unsigned int i = 0;
        i < element_array_positions_ordered_positions_map.size();
        i++
    ) {
        int j = element_array_positions_ordered_positions_map[i];
        positions[i] = new_ordered_positions[j];
    }

    load_position_bo();
}

void Drawable::load_element_array_bo()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_bo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        element_array.size() * sizeof(element_array[0]),
        &element_array[0],
        GL_STATIC_DRAW);
}

void Drawable::load_position_bo()
{
    glBindBuffer(GL_ARRAY_BUFFER, position_bo);
    glBufferData(
        GL_ARRAY_BUFFER,
        positions.size() * sizeof(positions[0]),
        &positions[0][0],
        GL_STATIC_DRAW);
}

void Drawable::load_normal_bo()
{
    glBindBuffer(GL_ARRAY_BUFFER, normal_bo);
    glBufferData(
        GL_ARRAY_BUFFER,
        normals.size() * sizeof(normals[0]),
        &normals[0][0],
        GL_STATIC_DRAW);
}

void Drawable::load_uv_bo()
{
    glBindBuffer(GL_ARRAY_BUFFER, uv_bo);
    glBufferData(
        GL_ARRAY_BUFFER,
        uvs.size() * sizeof(uvs[0]),
        &uvs[0][0],
        GL_STATIC_DRAW);
}

Drawable::~Drawable()
{
    glDeleteBuffers(1, &element_array_bo);
    glDeleteBuffers(1, &position_bo);
    glDeleteBuffers(1, &uv_bo);
    glDeleteBuffers(1, &normal_bo);
}

void Drawable::set_V(glm::mat4 new_V)
{
    V = new_V;
    glUniformMatrix4fv(V_id, 1, GL_FALSE, &V[0][0]);
}

void Drawable::set_P(glm::mat4 new_P)
{
    P = new_P;
    glUniformMatrix4fv(P_id, 1, GL_FALSE, &P[0][0]);
}

Drawable::Drawable(
    const char *bin_model_fname,
    GLuint program_id,
    GLuint vao,
    GLenum texture_unit,
    GLuint texture_id) :
        program_id(program_id),
        vao(vao),
        texture_unit(texture_unit),
        texture_id(texture_id)
{
    bin_obj_read(
        bin_model_fname,
        element_array,
        positions,
        uvs,
        normals,
        &ordered_positions,
        &element_array_positions_ordered_positions_map);

    glBindVertexArray(vao);
    glUseProgram(program_id);

    glGenBuffers(1, &element_array_bo);
    load_element_array_bo();

    glGenBuffers(1, &position_bo);
    load_position_bo();
    position_attrib = glGetAttribLocation(
        program_id,
        "position_modelspace");
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_attrib);

    glGenBuffers(1, &normal_bo);
    load_normal_bo();
    normal_attrib =  glGetAttribLocation(
        program_id,
        "normal_modelspace");
    glVertexAttribPointer(normal_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normal_attrib);

    glGenBuffers(1, &uv_bo);
    load_uv_bo();
    uv_attrib = glGetAttribLocation(
        program_id,
        "uv");
    glVertexAttribPointer(uv_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(uv_attrib);

    GLuint sampler_id = glGetUniformLocation(
        program_id,
        "texture_sampler");
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    // Set our sampler to use the texture unit
    glUniform1i(sampler_id, texture_id);

    M_id = glGetUniformLocation(program_id, "M");
    V_id = glGetUniformLocation(program_id, "V");
    P_id = glGetUniformLocation(program_id, "P");

    M = glm::mat4(1.0f);
    V = glm::mat4(1.0f);
    P = glm::mat4(1.0f);
}
