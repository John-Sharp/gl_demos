#include "mo_eng.hpp"
#include "../shader_compiler/shader_compiler.hpp"
#include "../utils/utils.hpp"
#include <string.h>
#include <iostream>
#include <libgen.h>

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

MoObject::~MoObject()
{
    delete model_unit;
    delete vslm_link;
    delete billboard;
}

void  MoObject::change_model(unsigned int new_model_index)
{
    model_index = new_model_index;
    model_unit->model = eng->model_templates[new_model_index].model;
    texture_index =
        eng->model_templates[new_model_index].textures[0];
    billboard->bb.mother_radius = eng->model_templates[new_model_index].r_bb;
    vslm_link->update_model_unit(*model_unit);
}

void MoObject::move(glm::vec3 direction_modelspace)
{
    GLfloat model_move_speed = 10.0f / eng->fps;
    if (direction_modelspace != glm::vec3(0.0f, 0.0f, 0.0f)) {
        direction_modelspace = glm::normalize(direction_modelspace);
    }

    model_unit->M = model_unit->M
        * glm::translate(glm::mat4(1), model_move_speed * direction_modelspace);
}

void MoObject::rotate(glm::vec3 rotation_axis)
{
    model_unit->M = model_unit->M
        * glm::rotate(glm::mat4(1), 0.1f, rotation_axis);
}

void MoObject::put_in_rotate_mode()
{
    mode = OBJECT_MODE_ROTATE;
    billboard->set_to_rotate();
}

void MoObject::put_in_active_mode()
{
    mode = OBJECT_MODE_ACTIVE;
    billboard->set_to_active();
    eng->input_processor->deactivate_state(DELETE_OBJECT);
}

void MoObject::put_in_normal_mode()
{
    mode = OBJECT_MODE_INACTIVE;
    billboard->set_to_normal();
}

MoObject *MoEng::add_object(unsigned int model_index) {
    unsigned int i;
    for (i = 0; i < NUMBER_OF_OBJECTS_ALLOWED; i++) {
        if (!indexed_objects[i]) {    
            break;
        }
    }

    MoObject *mobj = new MoObject(i);
    mobj->change_model(model_index);

    objects.push_back(mobj);
    indexed_objects[i] = mobj;

    set_active_object(mobj->object_index);
    mobj->model_unit->M =
        glm::affineInverse(view_unit.V)
        * glm::translate(glm::mat4(1), -1.0f * initial_camera_pos);

    return mobj;
}

void MoEng::set_active_object(unsigned int new_active_object_index)
{
    if (
        new_active_object_index >= NUMBER_OF_OBJECTS_ALLOWED
        || !indexed_objects[new_active_object_index]
    ) {
        return;
    } 

    active_object_index = new_active_object_index;
    active_object = indexed_objects[active_object_index];
    active_object->put_in_active_mode();

    input_processor->deactivate_state(ROTATE_MODE);
    input_processor->deactivate_state(DELETE_OBJECT);
}

void MoEng::move_active_object_on_request()
{
    if (!active_object) {
        return;
    }

    glm::vec3 move_dn(0.0f, 0.0f, 0.0f);

    if (input_processor->is_state_active(MV_RIGHT)) {
        move_dn += glm::vec3(1.0f, 0.0f, 0.0f);
    }

    if (input_processor->is_state_active(MV_LEFT)) {
        move_dn += glm::vec3(-1.0f, 0.0f, 0.0f);
    }

    if (input_processor->is_state_active(MV_UP)) {
        move_dn += glm::vec3(0.0f, 1.0f, 0.0f);
    }

    if (input_processor->is_state_active(MV_DOWN)) {
        move_dn += glm::vec3(0.0f, -1.0f, 0.0f);
    }

    if (input_processor->is_state_active(MV_FORWARD)) {
        move_dn += glm::vec3(0.0f, 0.0f, 1.0f);
    }

    if (input_processor->is_state_active(MV_BACKWARD)) {
        move_dn += glm::vec3(0.0f, 0.0f, -1.0f);
    }
    active_object->move(move_dn);
}

void MoEng::rotate_active_object_on_request()
{
    if (!active_object) {
        return;
    }

    if (input_processor->is_state_active(MV_RIGHT)) {
        active_object->rotate(glm::vec3(1.0f, 0.0f, 0.0f));
    }

    if (input_processor->is_state_active(MV_LEFT)) {
        active_object->rotate(glm::vec3(-1.0f, 0.0f, 0.0f));
    }

    if (input_processor->is_state_active(MV_UP)) {
        active_object->rotate(glm::vec3(0.0f, 1.0f, 0.0f));
    }

    if (input_processor->is_state_active(MV_DOWN)) {
        active_object->rotate(glm::vec3(0.0f, -1.0f, 0.0f));
    }

    if (input_processor->is_state_active(MV_FORWARD)) {
        active_object->rotate(glm::vec3(0.0f, 0.0f, 1.0f));
    }

    if (input_processor->is_state_active(MV_BACKWARD)) {
        active_object->rotate(glm::vec3(0.0f, 0.0f, -1.0f));
    }
}

void MoEng::delete_active_object()
{
    MoObject *object_to_be_deleted = active_object;
    indexed_objects[active_object->object_index] = NULL;
    objects.erase(
        std::remove(objects.begin(), objects.end(), active_object),
        objects.end());
    enter_global_mode();
    active_object = NULL;
    delete object_to_be_deleted;
}

void MoEng::change_active_object_model()
{
    unsigned int new_model_index = (active_object->model_index + 1)
        % NUMBER_OF_BASE_MODELS;
    active_object->change_model(new_model_index);
}

void MoEng::do_logic()
{
    if (active_object) {

        if (input_processor->is_state_active(ROTATE_MODE_SWITCH)) {
            input_processor->deactivate_state(ROTATE_MODE_SWITCH);
            if (input_processor->is_state_active(ROTATE_MODE)) {
                std::cout << "switching rotate mode on\n";
                active_object->put_in_rotate_mode();
            } else {
                std::cout << "switching rotate mode off\n";
                active_object->put_in_active_mode();
            }
        }

        if (input_processor->is_state_active(DELETE_OBJECT)) {
            input_processor->deactivate_state(DELETE_OBJECT);
            delete_active_object();
        }

        if (input_processor->is_state_active(GLOBAL_MODE_SWITCH)) {
            input_processor->deactivate_state(GLOBAL_MODE_SWITCH);
            enter_global_mode();
        }

        if (input_processor->is_state_active(CHANGE_MODEL)) {
            input_processor->deactivate_state(CHANGE_MODEL);
            change_active_object_model();
        }

        if (input_processor->is_state_active(ROTATE_MODE)) {
            rotate_active_object_on_request();
        } else { // just in normal move mode
            move_active_object_on_request();
        }
    } else { // in global mode
        if (input_processor->is_state_active(CHANGE_MODEL)) {
            input_processor->deactivate_state(CHANGE_MODEL);
            add_object(BASE_MODEL_TRIANGLE);
        }
    }

    read_for_requested_object();

}

void MoEng::create_key_bindings()
{
    input_processor->add_key_binding(SDLK_UP, MV_UP, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_DOWN, MV_DOWN, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_LEFT, MV_LEFT, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_RIGHT, MV_RIGHT, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_a, MV_FORWARD, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_s, MV_BACKWARD, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_n, CHANGE_TEXTURE, BINDING_CONTINUOUS);

    input_processor->add_key_binding(SDLK_1, PRESSED_1, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_2, PRESSED_2, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_3, PRESSED_3, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_4, PRESSED_4, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_5, PRESSED_5, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_6, PRESSED_6, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_7, PRESSED_7, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_8, PRESSED_8, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_9, PRESSED_9, BINDING_CONTINUOUS);
    input_processor->add_key_binding(SDLK_0, PRESSED_0, BINDING_CONTINUOUS);

    input_processor->add_key_binding(SDLK_r, ROTATE_MODE_SWITCH, BINDING_ONE_TIME);
    input_processor->add_key_binding(SDLK_r, ROTATE_MODE, BINDING_ATOMIC);

    input_processor->add_key_binding(SDLK_d, DELETE_OBJECT, BINDING_ONE_TIME);
    input_processor->add_key_binding(SDLK_ESCAPE, GLOBAL_MODE_SWITCH, BINDING_ONE_TIME);
    input_processor->add_key_binding(SDLK_m, CHANGE_MODEL, BINDING_ONE_TIME);
    input_processor->add_key_binding(SDLK_RETURN, SUBMIT_REQUEST, BINDING_ONE_TIME);
}

int MoEng::load_textures_for_model_template(
        mo_model_template *mo_template,
        json_object *texture_lib,
        json_object *textures_j)
{
    unsigned int num_textures = json_object_array_length(textures_j);

    for (unsigned int i = 0; i < num_textures; i++) {
        json_object *t_name_j =
            json_object_array_get_idx(textures_j, i);
        const char *t_name = json_object_get_string(t_name_j);

        json_object *t_id_j;
        if (!json_object_object_get_ex(texture_lib,
                    t_name, &t_id_j)) {
            fprintf(stderr,
                    "ERROR! No texture with name %s\n", t_name);
            return -1;
        }

        GLuint t_id = (GLuint)json_object_get_int64(t_id_j);
        mo_template->textures.push_back(t_id);
    }
    return 0;
}

int MoEng::setup_with_file(const char *setup_file_path)
{
    FILE *fp = fopen(setup_file_path, "r");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *setup_contents = new char[fsize + 1];
    fread(setup_contents, fsize, 1, fp);
    fclose(fp);
    setup_contents[fsize] = 0;
    json_object *jobj = json_tokener_parse(setup_contents);
    delete setup_contents;

    char *setup_file_path_cp = new char[strlen(setup_file_path) + 1];
    strcpy(setup_file_path_cp, setup_file_path);
    char *setup_file_path_dirname = dirname(setup_file_path_cp);
    char path_buffer[512];
    char dirname_buffer[512];
    memset(dirname_buffer, '\0', sizeof(dirname_buffer));
    strncpy(dirname_buffer, setup_file_path_dirname, sizeof(dirname_buffer));
    dirname_buffer[strlen(setup_file_path_dirname)] = '/';

    json_object *texture_lib;
    if (!json_object_object_get_ex(jobj, "texture_lib", &texture_lib)) {
        fprintf(stderr,
                "ERROR! No textures_lib attribute in set-up file\n");
        return -1;
    }

    if (json_object_get_type(texture_lib) != json_type_object) {
        fprintf(stderr, "ERROR! textures_lib attribute should be "
                "a json object in set-up file\n");
        return -1;
    }

    json_object_object_foreach(texture_lib, k, v){
        const char *texture_fname = json_object_get_string(v);
        memcpy(path_buffer, dirname_buffer, sizeof(dirname_buffer));
        const char *texture_path = strncat(
            path_buffer,
            texture_fname,
            sizeof(path_buffer));
        json_object_object_add(texture_lib, k, json_object_new_int64(
            load_texture(texture_path)));
    }

    json_object *models;
    if (!json_object_object_get_ex(jobj, "models", &models)) {
        fprintf(stderr,
                "ERROR! No models attribute in set-up file\n");
        return -1;
    }

    if (json_object_get_type(models) != json_type_array) {
        fprintf(stderr, "ERROR! models attribute should be "
                "a json array in set-up file\n");
        return -1;
    }

    unsigned int models_length = 
        (unsigned int)json_object_array_length(models);

    for (unsigned int i = 0; i < models_length; i++) {
        json_object *model_detail = json_object_array_get_idx(
                models, i);

        json_object *obj_file_fname_j;
        if (!json_object_object_get_ex(
                model_detail, "object_file", &obj_file_fname_j)) {
            fprintf(stderr, "ERROR! No object_file attribute in"
                    " model declaration\n");
            return -1;
        }
        const char *obj_file_fname =
            json_object_get_string(obj_file_fname_j);

        memcpy(path_buffer, dirname_buffer, sizeof(dirname_buffer));
        char *obj_file_fname_path = strncat(
                path_buffer, obj_file_fname, sizeof(path_buffer));
        model_templates[i].model = new BoalerModel(obj_file_fname_path);

        json_object *r_bb_j;
        if (!json_object_object_get_ex(
                model_detail, "radius_bb", &r_bb_j)) {
            fprintf(stderr, "ERROR! No radius_bb attribute in"
                    " model declaration\n");
            return -1;
        }
        model_templates[i].r_bb = json_object_get_double(r_bb_j);

        json_object *textures_j;
        if (!json_object_object_get_ex(
                model_detail, "textures", &textures_j)) {
            fprintf(stderr, "ERROR! No textures attribute in"
                    " model declaration\n");
            return -1;
        }
        if (load_textures_for_model_template(
            &model_templates[i],
            texture_lib,
            textures_j) == -1) {
            return -1;
        }
    }

    free(jobj);
    return 0;
}

MoEng::MoEng(
        const char*setup_file_path,
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
    active_object(NULL)
{
    input_processor = new GenInputProcessor<game_states>;
    base_input_processor = static_cast<BaseInputProcessor *>(input_processor);
    create_key_bindings();

    beng.reg_view_unit(&view_unit);

    if (setup_with_file(setup_file_path) == -1) {
        throw MoEngReturnException();
    }

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

    memset(
        indexed_objects,
        0,
        NUMBER_OF_OBJECTS_ALLOWED * sizeof(indexed_objects[0]));
}

void MoEng::enter_global_mode()
{
    if (!active_object) {
        return;
    }
    active_object->put_in_normal_mode();
    active_object = NULL;
    active_object_index = NUMBER_OF_OBJECTS_ALLOWED + 1;
}

void MoEng::check_digit(int value)
{
    unsigned int update_limit = 0.5f * fps;
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

    if (input_processor->is_state_active(digit)) {
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
    }
    return;    
}

void MoEng::read_for_requested_object()
{

    frames_since_last_entry += 1;
    for (int i = 0; i < 10; i++) {
        check_digit(i);
    }

    if (input_processor->is_state_active(SUBMIT_REQUEST)) {
        input_processor->deactivate_state(SUBMIT_REQUEST);
        object_index_being_requested -= 1;
        enter_global_mode();
        set_active_object(object_index_being_requested);
        object_index_being_requested = 0;
        last_digit_pressed = NO_DIGIT_PRESSED;
    }
}

void MoEng::render()
{
    for (
        std::vector<MoObject *>::iterator it = objects.begin();
        it != objects.end();
        ++it
    ) {
        (*it)->billboard->bb.update_pos();
    }

    beng.render();
}

void MoEng::process_input(SDL_Event *event)
{
    input_processor->process_input(event);
}
