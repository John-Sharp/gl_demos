#ifndef MO_BILLBOARD_H
#define MO_BILLBOARD_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#include "boaler.hpp"
#include "../text_texture/text_texture.hpp"
#include "../billboard/billboard.hpp"

#include "mo_eng.hpp"

typedef class MoEng MoEng;

class MoBillboard {
    public:

        static void prep(
            MoEng *eng,
            BoalerShaderUnit &shader_unit);

        MoBillboard(
            const BoalerVSLModelUnitLink &mother_vslm_link,
            int shape_id,
            GLfloat r_bb);

        static glm::mat4 V;
        static glm::mat4 P;
        static BoalerViewUnit *view_unit; 
        static BoalerModel *model;

        static BoalerVSLink *vs_link;

        static SDL_Surface *bb_bg_img;

        static int bb_w;
        static int bb_h;
        static SDL_Rect txt_rect;

        TextTexture text_texture;
        BoalerModelUnit model_unit;
        BoalerVSLModelUnitLink vslm_link;
        Billboard bb;
};
#endif
