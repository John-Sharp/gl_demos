#include "mo_eng.hpp"
#include "../shader_compiler/shader_compiler.hpp"
#include "../utils/utils.hpp"
#include <string.h>
#include <iostream>

MoEng *MoObject::eng = NULL;
void MoObject::prep(MoEng *objects_eng)
{
    eng = objects_eng;
}

MoObject::MoObject(unsigned int object_index) :
    object_index(object_index),
    model_index(0),       
    texture_index(eng->model_templates[model_index].textures[0]),
    shader_index(0)
{
    model_unit = new BoalerModelUnit(
        glm::mat4(1),
        0,
        texture_index,
        *eng->model_templates[model_index].model);

    vslm_link = new BoalerVSLModelUnitLink(
        *eng->view_shader_links[shader_index],
        *model_unit);

    billboard = new MoBillboard(
        *vslm_link,
        object_index + 1,
        eng->model_templates[model_index].r_bb);
}

void  MoObject::change_model(unsigned int new_model_index)
{
    model_index = new_model_index;
    model_unit->model = eng->model_templates[new_model_index].model;
    billboard->bb.mother_radius = eng->model_templates[new_model_index].r_bb;
    vslm_link->update_model_unit(*model_unit);
}

void MoObject::change_model_on_request()
{
    GenInputProcessor<game_states> *input_processor
        = static_cast<GenInputProcessor<game_states> *>(eng->input_processor);
    static bool model_should_change = true;

    if (input_processor->is_state_active(CHANGE_MODEL) && model_should_change) {
        unsigned int new_model_index = (model_index + 1) % NUMBER_OF_BASE_MODELS; 
        change_model(new_model_index);

        model_should_change = false;
        return;
    }

    if (!input_processor->is_state_active(CHANGE_MODEL)) {
        model_should_change = true;
    }
}

MoObject *MoEng::add_model(unsigned int model_index) {
    unsigned int i;
    for (i = 0; i < NUMBER_OF_MODELS_ALLOWED; i++) {
        if (!occupied_indices[i]) {    
            occupied_indices[i] = true;
            break;
        }
    }

    MoObject mobj(i);
    mobj.change_model(model_index);

    objects.push_back(mobj);
    indexed_objects[i] = &objects.back();

    return &objects.back();
}

void MoEng::set_active_object(unsigned int new_active_object_index)
{
    active_object_index = new_active_object_index;
    active_object = indexed_objects[active_object_index];
}

MoObject *MoEng::add_model_on_request()
{
    GenInputProcessor<game_states> *custom_input_processor
        = static_cast<GenInputProcessor<game_states> *>(input_processor);
    MoObject * mobj = NULL;

    if (
        custom_input_processor->is_state_active(CHANGE_MODEL)
        && active_object == NULL
    ) {
        mobj = add_model(BASE_MODEL_TRIANGLE);
    }

    if (!mobj) {
        return NULL;
    }

    set_active_object(mobj->object_index);
    // std::cout << view_unit.V[3][3] << std::endl;
    // mobj->model_unit->M = view_unit.P * glm::inverse(view_unit.P * view_unit.V);

    glm::mat4 I = view_unit.V * mobj->model_unit->M;
    I = mobj->model_unit->M;
    fprintf(stderr, 
            "%f  %f  %f  %f\n"
            "%f  %f  %f  %f\n"
            "%f  %f  %f  %f\n"
            "%f  %f  %f  %f\n\n",
            I[0][0], I[0][1], I[0][2], I[0][3],
            I[1][0], I[1][1], I[1][2], I[1][3],
            I[2][0], I[2][1], I[2][2], I[2][3],
            I[3][0], I[3][1], I[3][2], I[3][3]);
    // mobj->model_unit->M = glm::affineInverse(view_unit.V);

    return mobj;
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
            glm::vec3(0.0, 0.0, -1.0),
            glm::vec3(0.0, 0.3, 0.0),
            glm::vec3(0.0, 1.0, 0.0)),
        glm::perspective(44.9f, (float)w / (float)h, 0.1f, 100.0f))),
    active_object_index(0),
    active_object(NULL)
{
    GenInputProcessor<game_states> *custom_ip = new GenInputProcessor<game_states>;
    input_processor = static_cast<BaseInputProcessor *>(custom_ip);

    custom_ip->add_key_binding(SDLK_UP, MV_UP);
    custom_ip->add_key_binding(SDLK_DOWN, MV_DOWN);
    custom_ip->add_key_binding(SDLK_LEFT, MV_LEFT);
    custom_ip->add_key_binding(SDLK_RIGHT, MV_RIGHT);
    custom_ip->add_key_binding(SDLK_a, MV_FORWARD);
    custom_ip->add_key_binding(SDLK_s, CHANGE_TEXTURE);
    custom_ip->add_key_binding(SDLK_m, CHANGE_MODEL);

    custom_ip->add_key_binding(SDLK_ESCAPE, GLOBAL_MODE);
    custom_ip->add_key_binding(SDLK_1, SELECTED_1);


    beng.reg_view_unit(&view_unit);

    // Set up model template library
    // BASE_MODEL_TRIANGLE:
    model_templates[BASE_MODEL_TRIANGLE].model = new BoalerModel("triangle.bin");
    model_templates[BASE_MODEL_TRIANGLE].r_bb = 0.8f;
    model_templates[BASE_MODEL_TRIANGLE].textures.push_back(load_texture("resources/test_texture.png"));
    model_templates[BASE_MODEL_TRIANGLE].textures.push_back(load_texture("resources/test_texture2.png"));

    // BASE_MODEL_RECTANGLE:
    model_templates[BASE_MODEL_RECTANGLE].model = new BoalerModel("rectangle.bin");
    model_templates[BASE_MODEL_RECTANGLE].r_bb = 1.0f;
    model_templates[BASE_MODEL_RECTANGLE].textures.push_back(load_texture("resources/test_texture.png"));
    model_templates[BASE_MODEL_RECTANGLE].textures.push_back(load_texture("resources/test_texture2.png"));

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

    bool temp = false;
    memcpy(
        occupied_indices,
        &temp,
        NUMBER_OF_MODELS_ALLOWED * sizeof(temp));

    MoObject *temp_mobj = NULL;
    memcpy(
        indexed_objects,
        &temp_mobj,
        NUMBER_OF_MODELS_ALLOWED * sizeof(temp_mobj));
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

    if (active_object) {
        active_object->change_model_on_request();
    } else {
        add_model_on_request();
    }

    beng.render();
}

void MoEng::process_input(SDL_Event *event)
{
    GenInputProcessor<game_states> *custom_input_processor
        = static_cast<GenInputProcessor<game_states> *>(input_processor);
    custom_input_processor->process_input(event);
}
