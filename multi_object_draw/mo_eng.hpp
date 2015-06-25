#ifndef MO_ENG_H
#define MO_ENG_H

#include "../utils/base_eng/base_eng.hpp"
#include "boaler.hpp"
#include "mo_billboard.hpp"
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

enum {
    NUMBER_OF_BASE_MODELS = 2,
    NUMBER_OF_SHADERS = 1,
    NUMBER_OF_MODELS_ALLOWED = 9
};

enum {
    BASE_MODEL_TRIANGLE = 0,
    BASE_MODEL_RECTANGLE = 1
};

enum game_states { MV_UP, MV_DOWN, MV_LEFT, MV_RIGHT, MV_FORWARD,
    MV_BACKWARD,
    CHANGE_TEXTURE, CHANGE_MODEL, GLOBAL_MODE, ROTATE_MODE,
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
        static void prep(MoEng *eng);
        void change_model(unsigned int new_model_index);
        void change_model_on_request();
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
            int w,
            int h,
            const char *window_title,
            unsigned int fps);

        glm::vec3 initial_camera_pos;

        void enter_global_mode();
        void enter_global_mode_on_request();
        void check_digit(int value);
        void read_for_requested_object();
        void render();
        void process_input(SDL_Event *event);
        MoObject *add_model(unsigned int model_index);
        void set_active_object(
            unsigned int new_active_object_index);
        MoObject *add_model_on_request();
        void move_active_object_on_request();
        void rotate_active_object_on_request();

        BoalerEng beng;
        BoalerShaderUnit billboard_shader_unit;
        mo_model_template model_templates[NUMBER_OF_BASE_MODELS];
        BoalerShaderUnit *shaders[NUMBER_OF_SHADERS];
        BoalerViewUnit view_unit;
        BoalerVSLink *view_shader_links[NUMBER_OF_SHADERS];

        unsigned int object_index_being_requested;
        unsigned int frames_since_last_entry;
        game_states last_digit_pressed;

        unsigned int active_object_index;
        MoObject *active_object;
        unsigned int number_of_objects;
        MoObject *indexed_objects[NUMBER_OF_MODELS_ALLOWED];
        bool occupied_indices[NUMBER_OF_MODELS_ALLOWED];
};

#endif
