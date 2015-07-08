#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "../multi_object_draw/boaler.hpp"

class SkyBox {
    SkyBox(
        BoalerEng &beng,
        GLuint texture_ref,
        const BoalerViewUnit &view_unit_to_track,
        const BoalerShaderUnit &shader_unit);
    ~SkyBox();

    void update();

    const BoalerViewUnit *view_unit_to_track;
    BoalerVSLModelUnitLink *vslmu;
};

SkyBox::SkyBox(
        GLuint texture_ref,
        BoalerViewUnit *view_unit_to_track,
        BoalerShaderUnit *shader_unit
) :
    view_unit_to_track(*view_unit_to_track)
{

    BoalerModel *model = new BoalerModel("cube.bin");

    BoalerModelUnit *model_unit = new BoalerModelUnit(
        glm::mat4(1),
        0,
        texture_ref,
        *model);

    BoalerVSLink *vs_link = new BoalerVSLink(


    
}
#endif
