#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "../multi_object_draw/boaler.hpp"

class Billboard {
    public:
        Billboard(
            const BoalerVSLModelUnitLink &mother,
            BoalerVSLModelUnitLink &bb_vsl_model_link,
            GLfloat mother_radius
            );

        const BoalerVSLModelUnitLink &mother;
        BoalerVSLModelUnitLink &bb_vsl_model_link;
        GLfloat mother_radius;

        glm::mat4 M_pre_translate;

        void update_pos();
};

#endif
