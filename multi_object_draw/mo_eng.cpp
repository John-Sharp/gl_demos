#include "mo_eng.hpp"
#include "../shader_compiler/shader_compiler.hpp"
#include "../utils/utils.hpp"

enum {
    BASE_MODEL_TRIANGLE = 0,
    BASE_MODEL_RECTANGLE = 1
};

MoObject::prep(MoEng *objects_eng)
{
    eng = objects_eng;
}

MoObject::MoObject() :
    model_index(0),        
    texture_index(eng.textures),
{}

MoEng::MoEng(
        int w,
        int h,
        const char *window_title,
        unsigned int fps,
        BaseInputProcessor *input_processor) :
    BaseEng(w, h, window_title, fps, input_processor)
{
    // Set up model library
    models[BASE_MODEL_TRIANGLE] = new BoalerModel("triangle.bin");
    models[BASE_MODEL_RECTANGLE] = new BoalerModel("rectangle.bin");

    // Set up texture library for each base model
    // BASE_MODEL_TRIANGLE:
    model_textures[BASE_MODEL_TRIANGLE].push_back(load_texture("resources/test_texture.png"));
    model_textures[BASE_MODEL_TRIANGLE].push_back(load_texture("resources/test_texture2.png"));
    // BASE_MODEL_RECTANGLE:
    model_textures[BASE_MODEL_RECTANGLE].push_back(load_texture("resources/test_texture.png"));
    model_textures[BASE_MODEL_RECTANGLE].push_back(load_texture("resources/test_texture2.png"));

    // Set up shader unit library
    GLuint shader_program = compile_shader(
        "resources/basic_shading.vertexshader",
        "resources/basic_shading.fragmentshader");

    shaders[0] = new BoalerShaderUnit(shader_program);

    // Set up some sample objects
    objects.push_back(MoObject());
    objects.push_back(MoObject());
    objects[1].setModel(*models[BASE_MODEL_RECTANGLE]);
}
