#ifndef MO_ENG_H
#define MO_ENG_H

#include "../utils/base_eng/base_eng.hpp"
#include "boaler.hpp"

enum {
    NUMBER_OF_BASE_MODELS = 2,
    NUMBER_OF_SHADERS = 1
};

class MoEng : public BaseEng {
    public:

        MoEng(
            int w,
            int h,
            const char *window_title,
            unsigned int fps,
            BaseInputProcessor *input_processor);

        BoalerModel *models[NUMBER_OF_BASE_MODELS];
        std::vector<GLuint> model_textures[NUMBER_OF_BASE_MODELS];
        BoalerShaderUnit *shaders[NUMBER_OF_SHADERS];

        std::vector <MoObject> objects;
};

#endif
