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

    MoObject *mobj = new MoObject(i);
    mobj->change_model(model_index);

    number_of_objects += 1;
    indexed_objects[i] = mobj;

    return mobj;
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
    mobj->model_unit->M =
        glm::affineInverse(view_unit.V)
        * glm::translate(glm::mat4(1), -1.0f * initial_camera_pos);

    glm::mat4 I = view_unit.V * mobj->model_unit->M;
    I = mobj->model_unit->M;

    return mobj;
}

MoEng::MoEng(
        int w,
        int h,
        const char *window_title,
        unsigned int fps) : 
    BaseEng(w, h, window_title, fps, NULL),
    initial_camera_pos(glm::vec3(0.0f, 0.0f, 4.0f)),
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
    object_index_being_requested(0),
    frames_since_last_entry(0),
    last_digit_pressed(NO_DIGIT_PRESSED),
    active_object_index(0),
    active_object(NULL),
    number_of_objects(0)
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
    custom_ip->add_key_binding(SDLK_RETURN, SUBMIT_REQUEST);
    custom_ip->add_key_binding(SDLK_1, PRESSED_1);
    custom_ip->add_key_binding(SDLK_2, PRESSED_2);
    custom_ip->add_key_binding(SDLK_3, PRESSED_3);
    custom_ip->add_key_binding(SDLK_4, PRESSED_4);
    custom_ip->add_key_binding(SDLK_5, PRESSED_5);
    custom_ip->add_key_binding(SDLK_6, PRESSED_6);
    custom_ip->add_key_binding(SDLK_7, PRESSED_7);
    custom_ip->add_key_binding(SDLK_8, PRESSED_8);
    custom_ip->add_key_binding(SDLK_9, PRESSED_9);
    custom_ip->add_key_binding(SDLK_0, PRESSED_0);

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

void MoEng::enter_global_mode()
{
    active_object = NULL;
    active_object_index = NUMBER_OF_MODELS_ALLOWED + 1;
    object_index_being_requested = 0;
}

void MoEng::enter_global_mode_on_request()
{
    GenInputProcessor<game_states> *custom_input_processor
        = static_cast<GenInputProcessor<game_states> *>(input_processor);

    if (custom_input_processor->is_state_active(GLOBAL_MODE)) {
        enter_global_mode();
    }
}

void MoEng::check_digit(int value)
{
    unsigned int update_limit = 0.5f * fps;
    GenInputProcessor<game_states> *custom_input_processor
        = static_cast<GenInputProcessor<game_states> *>(input_processor);
    game_states digits[10] = {
        PRESSED_0,
        PRESSED_1,
        PRESSED_2,
        PRESSED_3,
        PRESSED_4,
        PRESSED_5,
        PRESSED_6,
        PRESSED_7,
        PRESSED_8,
        PRESSED_9
    };
    game_states digit = digits[value];

    if (custom_input_processor->is_state_active(digit)) {
        if (
            frames_since_last_entry < update_limit
            && last_digit_pressed == digit
        ) {
            return;
        }
        last_digit_pressed = digit;
        frames_since_last_entry = 0;

        // update requested number
        object_index_being_requested =
            object_index_being_requested * 10
            + value; 
        std::cout << "Requesting: " << object_index_being_requested
            << '\n';
    }
    return;    
}

void MoEng::read_for_requested_object()
{
    GenInputProcessor<game_states> *custom_input_processor
        = static_cast<GenInputProcessor<game_states> *>(input_processor);

    if (custom_input_processor->is_state_active(GLOBAL_MODE)) {
        object_index_being_requested = 0;
        last_digit_pressed = NO_DIGIT_PRESSED;

        return;
    }

    frames_since_last_entry += 1;
    for (int i = 0; i < 10; i++) {
        check_digit(i);
    }

    if (custom_input_processor->is_state_active(SUBMIT_REQUEST)) {
        object_index_being_requested -= 1;
        std::cout << "Requested index " << object_index_being_requested << '\n';
        if (object_index_being_requested < number_of_objects) {
            std::cout << "here, setting active object " << object_index_being_requested << '\n';
            set_active_object(object_index_being_requested);
        }
        object_index_being_requested = 0;
        last_digit_pressed = NO_DIGIT_PRESSED;
    }
}

void MoEng::render()
{
    for (unsigned int i = 0; i < number_of_objects; i++ ) {
        indexed_objects[i]->billboard->bb.update_pos();
    }

    read_for_requested_object();
    if (active_object) {
        enter_global_mode_on_request();
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
