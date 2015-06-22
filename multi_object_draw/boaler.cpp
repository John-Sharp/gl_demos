#include "boaler.hpp"
#include "../simple_obj_reader/bin_obj_reader.hpp"
#include <stdio.h>

typedef class BoalerVSLink BoalerVSLink;

BoalerModel::BoalerModel(
    const char *bin_model_fname)
{
    bin_obj_read(
        bin_model_fname,
        element_array,
        positions,
        uvs,
        normals,
        &ordered_positions,
        &element_array_positions_ordered_positions_map);

    glGenBuffers(1, &element_array_bo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_bo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        element_array.size() * sizeof(element_array[0]),
        &element_array[0],
        GL_STATIC_DRAW);

    glGenBuffers(1, &position_bo);
    glBindBuffer(GL_ARRAY_BUFFER, position_bo);
    glBufferData(
        GL_ARRAY_BUFFER,
        positions.size() * sizeof(positions[0]),
        &positions[0][0],
        GL_STATIC_DRAW);

    glGenBuffers(1, &normal_bo);
    glBindBuffer(GL_ARRAY_BUFFER, normal_bo);
    glBufferData(
        GL_ARRAY_BUFFER,
        normals.size() * sizeof(normals[0]),
        &normals[0][0],
        GL_STATIC_DRAW);

    glGenBuffers(1, &uv_bo);
    glBindBuffer(GL_ARRAY_BUFFER, uv_bo);
    glBufferData(
        GL_ARRAY_BUFFER,
        uvs.size() * sizeof(uvs[0]),
        &uvs[0][0],
        GL_STATIC_DRAW);
}

BoalerModelUnit::BoalerModelUnit(
    glm::mat4 M,
    unsigned int texture_unit_index,
    GLuint texture_id,
    BoalerModel &model) :
        M(M),
        texture_unit_index(texture_unit_index),
        texture_id(texture_id),
        model(&model)
{}

BoalerVSLModelUnitLink::BoalerVSLModelUnitLink(
    BoalerVSLink &vs_link,
    BoalerModelUnit &model_unit) :
        vs_link(&vs_link),
        model_unit(&model_unit)
{
    vs_link.model_unit_links.push_back(this);

    glGenVertexArrays(1, &vao);

    update_model_unit(model_unit);
}

void BoalerVSLModelUnitLink::update_model_unit(
        BoalerModelUnit &new_model_unit
) {
    model_unit = &new_model_unit;

    glBindVertexArray(vao);

    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        model_unit->model->element_array_bo);

    glBindBuffer(GL_ARRAY_BUFFER, model_unit->model->position_bo);
    glVertexAttribPointer(
        vs_link->shader_unit->position_attr,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        0);
    glEnableVertexAttribArray(vs_link->shader_unit->position_attr);

    glBindBuffer(GL_ARRAY_BUFFER, model_unit->model->uv_bo);
    glVertexAttribPointer(
        vs_link->shader_unit->uv_attr,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        0);
    glEnableVertexAttribArray(vs_link->shader_unit->uv_attr);

    glBindBuffer(GL_ARRAY_BUFFER, model_unit->model->normal_bo);
    glVertexAttribPointer(
        vs_link->shader_unit->normal_attr,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        0);
    glEnableVertexAttribArray(vs_link->shader_unit->normal_attr);

    glBindVertexArray(0);
}

void BoalerVSLModelUnitLink::model_unit_linker()
{
    glBindVertexArray(vao);

    glUniformMatrix4fv(
        vs_link->shader_unit->M_unfm,
        1,
        GL_FALSE,
        &(model_unit->M[0][0]));

    glActiveTexture(GL_TEXTURE0 + model_unit->texture_unit_index);
    glBindTexture(GL_TEXTURE_2D, model_unit->texture_id);
    glUniform1i(
        vs_link->shader_unit->texture_sampler_unfm,
        model_unit->texture_unit_index);

}

void BoalerVSLModelUnitLink::render()
{
    glBindVertexArray(vao);

    glDrawElements(
        GL_TRIANGLES,
        model_unit->model->element_array.size(),
        GL_UNSIGNED_INT,
        0);

    glBindVertexArray(0);
}

BoalerShaderUnit::BoalerShaderUnit(
    GLuint program_id) :
        program_id(program_id)
{
    position_attr = glGetAttribLocation(
        program_id,
        "position_modelspace");

    uv_attr = glGetAttribLocation(
        program_id,
        "uv");

    normal_attr = glGetAttribLocation(
        program_id,
        "normal_modelspace");

    M_unfm = glGetUniformLocation(
        program_id,
        "M");

    V_unfm = glGetUniformLocation(
        program_id,
        "V");

    P_unfm = glGetUniformLocation(
        program_id,
        "P");

    texture_sampler_unfm = glGetUniformLocation(
        program_id,
        "texture_sampler");
}

BoalerViewUnit::BoalerViewUnit(
    glm::mat4 V,
    glm::mat4 P) :
        V(V),
        P(P)
{}

void BoalerViewUnit::render()
{
    for (unsigned int i = 0; i < shader_links.size(); i++) {
        shader_links[i]->shader_unit_linker();
        for (
            unsigned int j = 0;
            j < shader_links[i]->model_unit_links.size();
            j++
        ) {
            shader_links[i]->model_unit_links[j]->model_unit_linker();
            shader_links[i]->model_unit_links[j]->render();
        }
    }
}

BoalerVSLink::BoalerVSLink(
    BoalerViewUnit &view_unit,
    BoalerShaderUnit &shader_unit) :
        view_unit(&view_unit),
        shader_unit(&shader_unit)
{
    view_unit.shader_links.push_back(this);
}

void BoalerVSLink::shader_unit_linker()
{
    glUseProgram(shader_unit->program_id);
    glUniformMatrix4fv(
        shader_unit->V_unfm,
        1,
        GL_FALSE,
        &(view_unit->V[0][0]));

    glUniformMatrix4fv(
        shader_unit->P_unfm,
        1,
        GL_FALSE,
        &(view_unit->P[0][0]));
}

void BoalerEng::reg_view_unit(BoalerViewUnit *vu)
{

    view_units.push_back(vu);
}

void BoalerEng::reg_shader_unit(BoalerShaderUnit *su)
{
    shader_units.push_back(su);
}

void BoalerEng::reg_model(BoalerModel *m)
{
    models.push_back(m);
}

void BoalerEng::reg_model_unit(BoalerModelUnit *mu)
{
    model_units.push_back(mu);
}

void BoalerEng::render()
{
    for (unsigned int i = 0; i < view_units.size(); i++) {
        view_units[i]->render();
    }
}
