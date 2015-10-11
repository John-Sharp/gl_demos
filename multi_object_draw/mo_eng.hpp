#ifndef MO_ENG_H
#define MO_ENG_H

#include "../utils/base_eng/base_eng.hpp"
#include "../boaler/boaler.hpp"
#include "mo_billboard.hpp"
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <json-c/json.h>

#include <exception>

struct MoEngReturnException : public std::exception
{};

enum {
    NUMBER_OF_SHADERS = 1,
    NUMBER_OF_OBJECTS_ALLOWED = 9
};

enum {
    BASE_MODEL_TRIANGLE = 0,
    BASE_MODEL_RECTANGLE = 1
};

enum game_states { MV_UP, MV_DOWN, MV_LEFT, MV_RIGHT, MV_FORWARD,
    MV_BACKWARD,
    CHANGE_TEXTURE, CHANGE_MODEL, GLOBAL_MODE,
    ROTATE_MODE_SWITCH, ROTATE_MODE,
    DELETE_OBJECT,
    GLOBAL_MODE_SWITCH,
    SUBMIT_REQUEST,
    NO_DIGIT_PRESSED,
    PRESSED_1,
    PRESSED_2,
    PRESSED_3,
    PRESSED_4,
    PRESSED_5,
    PRESSED_6,
    PRESSED_7,
    PRESSED_8,
    PRESSED_9,
    PRESSED_0 };

enum object_mode {
    OBJECT_MODE_INACTIVE,
    OBJECT_MODE_ACTIVE,
    OBJECT_MODE_ROTATE
};

typedef class MoBillboard MoBillboard;
typedef class MoEng MoEng;

class MoObject {
    public:
        MoObject(unsigned int object_index);
        ~MoObject();
        static void prep(MoEng *eng);
        void change_model(unsigned int new_model_index);
        void change_texture();
        void move(glm::vec3 direction_modelspace);
        void rotate(glm::vec3 rotation_axis);
        void put_in_rotate_mode();
        void put_in_active_mode();
        void put_in_normal_mode();
        object_mode mode;
            
        static MoEng *eng;
        unsigned int object_index;

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
            const char *setup_file_path,
            int w,
            int h,
            const char *window_title,
            unsigned int fps);

        glm::vec3 initial_camera_pos;

        void create_key_bindings();

        int load_textures_for_model_template(
            mo_model_template *mo_template,
            json_object *texture_lib,
            json_object *textures_j);
        int setup_with_file(const char*setup_file_path);

        void enter_global_mode();
        void check_digit(int value);
        void read_for_requested_object();
        void render();
        void process_input(SDL_Event *event);
        MoObject *add_object(unsigned int model_index);
        void set_active_object(
            unsigned int new_active_object_index);
        void move_active_object_on_request();
        void rotate_active_object_on_request();
        void delete_active_object();
        void change_active_object_model();

        void do_logic();

        GenInputProcessor<game_states> *input_processor;

        BoalerEng beng;
        BoalerShaderUnit billboard_shader_unit;
        std::vector<mo_model_template> model_templates;
        BoalerShaderUnit *shaders[NUMBER_OF_SHADERS];
        BoalerViewUnit view_unit;
        BoalerVSLink *view_shader_links[NUMBER_OF_SHADERS];

        unsigned int object_index_being_requested;
        unsigned int frames_since_last_entry;
        game_states last_digit_pressed;

        unsigned int active_object_index;
        MoObject *active_object;
        std::vector<MoObject *> objects;
        MoObject *indexed_objects[NUMBER_OF_OBJECTS_ALLOWED];
};

#endif
