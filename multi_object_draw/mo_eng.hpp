#ifndef MO_ENG_H
#define MO_ENG_H

#include "../utils/base_eng/base_eng.hpp"
#include "boaler.hpp"
#include "mo_billboard.hpp"
#include <vector>

enum {
    NUMBER_OF_BASE_MODELS = 2,
    NUMBER_OF_SHADERS = 1
};

enum {
    BASE_MODEL_TRIANGLE = 0,
    BASE_MODEL_RECTANGLE = 1
};

enum game_states { MV_UP, MV_DOWN, MV_LEFT, MV_RIGHT, MV_FORWARD,
    CHANGE_TEXTURE, CHANGE_MODEL };

typedef class MoBillboard MoBillboard;
typedef class MoEng MoEng;

class MoObject {
    public:
        MoObject();
        static void prep(MoEng *eng);
        void change_model(unsigned int new_model_index);
        void change_model_on_request();
            
        static MoEng *eng;
        unsigned int model_index;
        unsigned int texture_index;
        unsigned int shader_index;
        BoalerModelUnit *model_unit;
        BoalerVSLModelUnitLink *vslm_link;
        MoBillboard *billboard;

};

struct mo_model_template {
    BoalerModel *model;
    GLfloat r_bb;
    std::vector<GLuint> textures;
};

class MoEng : public BaseEng {
    public:

        MoEng(
            int w,
            int h,
            const char *window_title,
            unsigned int fps);
        void render();
        void process_input(SDL_Event *event);
        MoObject &add_model(unsigned int model_index);

        BoalerEng beng;
        BoalerShaderUnit billboard_shader_unit;
        mo_model_template model_templates[NUMBER_OF_BASE_MODELS];
        BoalerShaderUnit *shaders[NUMBER_OF_SHADERS];
        BoalerViewUnit view_unit;
        BoalerVSLink *view_shader_links[NUMBER_OF_SHADERS];

        std::vector<MoObject> objects;
};

#endif
