#include "mo_eng.hpp"
#include "../shader_compiler/shader_compiler.hpp"
#include "../utils/utils.hpp"

MoEng *MoObject::eng = NULL;
void MoObject::prep(MoEng *objects_eng)
{
    eng = objects_eng;
}

MoObject::MoObject() :
    model_index(0),       
    texture_index(eng->model_textures[0][0]),
    shader_index(0)
{
    model_unit = new BoalerModelUnit(
        glm::mat4(1),
        0,
        texture_index,
        *eng->models[model_index]);

    vslm_link = new BoalerVSLModelUnitLink(
        *eng->view_shader_links[shader_index],
        *model_unit);

    billboard = new MoBillboard(*vslm_link, 0, 0.8f);
}

void  MoObject::change_model(unsigned int model_index)
{
    model_unit->model = eng->models[model_index];
    vslm_link->update_model_unit(*model_unit);
}

MoObject &MoEng::add_model(unsigned int model_index)
{
    MoObject mobj;
    mobj.change_model(model_index);

    objects.push_back(mobj);

    return objects.back();
}

MoEng::MoEng(
        int w,
        int h,
        const char *window_title,
        unsigned int fps) : 
    BaseEng(w, h, window_title, fps, NULL),
    beng(BoalerEng()),
    billboard_shader_unit(compile_shader(
        "resources/basic_shading.vertexshader",
        "resources/basic_shading.fragmentshader")),
    view_unit(BoalerViewUnit(
        glm::lookAt(
            glm::vec3(0.0, 0.0, 4.0),
            glm::vec3(0.0, 0.0, -1.0),
            glm::vec3(0.0, 1.0, 0.0)),
        glm::perspective(44.9f, (float)w / (float)h, 0.1f, 100.0f)))
{
    GenInputProcessor<game_states> *custom_ip = new GenInputProcessor<game_states>;
    input_processor = static_cast<BaseInputProcessor *>(custom_ip);

    custom_ip->add_key_binding(SDLK_UP, MV_UP);
    custom_ip->add_key_binding(SDLK_DOWN, MV_DOWN);
    custom_ip->add_key_binding(SDLK_LEFT, MV_LEFT);
    custom_ip->add_key_binding(SDLK_RIGHT, MV_RIGHT);
    custom_ip->add_key_binding(SDLK_a, MV_FORWARD);
    custom_ip->add_key_binding(SDLK_s, CHANGE_TEXTURE);


    beng.reg_view_unit(&view_unit);

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

    view_shader_links[0] = new BoalerVSLink(view_unit, *shaders[0]);

    // Prepare the static variables of billboard
    // and MoObject
    MoBillboard::prep(this, billboard_shader_unit);
    MoObject::prep(this);
}

void MoEng::render()
{
    for (
        std::vector<MoObject>::iterator i = objects.begin();
        i != objects.end();
        i++
    ) {
        i->billboard->bb.update_pos();
    }

    beng.render();
}

void MoEng::process_input(SDL_Event *event)
{
    GenInputProcessor<game_states> *custom_input_processor
        = static_cast<GenInputProcessor<game_states> *>(input_processor);
    custom_input_processor->process_input(event);
}
