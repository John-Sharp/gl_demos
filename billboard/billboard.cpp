#include "billboard.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <stdio.h>

Billboard::Billboard(
    const BoalerVSLModelUnitLink &mother,
    BoalerVSLModelUnitLink &bb_vsl_model_link,
    GLfloat mother_radius
    ) :
        mother(&mother),
        bb_vsl_model_link(&bb_vsl_model_link),
        mother_radius(mother_radius),
        M_pre_translate(bb_vsl_model_link.model_unit->M)
{}
        
void Billboard::update_pos()
{
    const glm::mat4 &mother_V = mother->vs_link->view_unit->V;
    const glm::mat4 &mother_P = mother->vs_link->view_unit->P;
    const glm::mat4 &mother_M = mother->model_unit->M;

    glm::vec4 bb_centre_point_screen_space = mother_P * mother_V
        * mother_M * glm::vec4(0.0f, mother_radius, 0.0f, 1.0f);

    bb_centre_point_screen_space /= bb_centre_point_screen_space.w;
    bb_centre_point_screen_space.z = 0;

    const glm::mat4 &bb_P = bb_vsl_model_link->vs_link->view_unit->P;
    bb_centre_point_screen_space = glm::inverseTranspose(bb_P)
        * bb_centre_point_screen_space;

    bb_vsl_model_link->model_unit->M = glm::translate(
        glm::mat4(1),
        glm::vec3(bb_centre_point_screen_space))
            * M_pre_translate;
}
