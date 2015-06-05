
#include "rengine.hpp"

enum { WIN_W = 800, WIN_H = 600, FPS = 100 };

typedef class BoalerVSLink BoalerVSLink;

// Tyedef of a pointer to a handler function that gets called
// during the linking of view unit member variables and shader
// uniform attributes. Use this for linking custom view unit member
// variables with shader uniform attributes
typedef void (*pre_shader_unit_render_handler)(BoalerVSLink &);

// Tyedef of a pointer to a handler function that gets called
// during the linking of model unit member variables and shader
// uniform attributes and vertex attributes.
// Use this for linking custom model unit member
// variables with shader uniform attributes
typedef void (*pre_model_unit_render_handler)(BoalerVSLModelUnitLink &);

// BoalerModel
class BoalerModel
{
    public:
        // std::vector and buffer object for storing the element
        // array data
        std::vector<GLuint> element_array;
        GLuint element_array_bo;

        // std::vector of positions ordered in the same way that they
        // are declared in the *.obj file that contains the model
        std::vector<glm::vec3> ordered_positions;

        // std::vector and buffer object
        // for storing the position vertex data
        std::vector<glm::vec3> positions;
        GLuint position_bo;

        // std::vector with size = positions.size() where each element
        // gives the index of the ordered_positions
        // vertex that is the same as the positions vertex corresponding
        // to that element
        std::vector<GLuint> element_array_positions_ordered_positions_map;

        // std::vector, buffer object and shader attribute variable
        // for storing the texture coordinate data
        std::vector<glm::vec2> uvs;
        GLuint uv_bo;

        // std::vector, buffer object and shader attribute variable
        // for storing the normals coordinate data
        std::vector<glm::vec3> normals;
        GLuint normal_bo;
 
    private:

};

class BoalerModelInstance
{
    public:
        // Model instances model matrix
        glm::mat4 M;

        // Reference to the linked model
        BoalerModel &model;
};

class BoalerVSLModelUnitLink
{
    public:
        // References to the linked vs link and model unit
        BoalerVSLink &vs_link;
        BoalerModelUnit &model_unit;
        
        // Reference to this vertex array object that links this
        // model unit with the shader referenced through this link
        GLuint vao;

        // Handler method that gets called just prior 
        // to the rendering of each model unit
        void pre_model_unit_render_handler handler;

        void render();
}

class BoalerShaderUnit
{
    public:
        // Reference to shader unit's program
        GLuint program_id;

        // References to model unit's shader's position, uv and
        // normal attributes
        GLint position_attr;
        GLint uv_attr;
        GLint normal_attr;

        // References to shader program's M, V and P
        // uniforms
        GLuint M_id; 
        GLuint V_unfm;
        GLuint P_unfm;

        // Reference to shader unit's engine
        BoalerEng &engine;

    private:

}

class BoalerViewUnit
{
    public:
        // vector to list of view unit's shader units
        std::vector<BoalerVSLink> shader_links;

        // view unit's view and projection matrices
        glm::mat4 V;
        glm::mat4 P;

        void link_shader(BoalerShaderUnit &shader_unit, view_shader_link_fn link_fn);
        void render();

    private:

};

class BoalerVSLink
{
    public:
        // References to the link's view and shader units
        BoalerViewUnit &view_unit;
        BoalerShaderUnit &shader_unit;

        virtual void shader_unit_linker();

        // list of VS link - model unit links that should be rendered with this shader and view unit
        std::vector<BoalerVSLModelUnitLink> model_units_links;
};

BoalerViewUnit::render()
{
    for (unsigned int i = 0; i < shader_links.size(); i++) {
        // set the shader's view and projection matrices
        BoalerVSLink &shader_link = shader_links[i];
        BoalerShaderUnit &shader_unit = shader_link.shader_unit;
        GLuint V_id = shader_unit.V_id;
        GLuint P_id = shader_unit.P_id;
        glUniformMatrix4fv(V_id, 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(P_id, 1, GL_FALSE, &P[0][0]);
        shader_link.handler(shader_link);
        for (unsigned int j = 0; j < shader_link.model_units_links.size(); j++) {
            BoalerShaderModelInstanceLink &model_instance_link = \
                shader_link.model_instance_links[j].model_instance;
            model_instance.render();
        }
    }
}

class BoalerEng
{
    public:
        std::vector<BoalerModel *> models;
        std::vector<BoalerModelInstance *> model_instances;
        std::vector<BoalerSharderUnit *> shader_units;
        std::vector<BoalerViewUnit *> view_units;

        void reg_view_unit(BoalerViewUnit *vu);
        void render();

    private:

};

BoalerEng::render()
{
    for (unsigned int i = 0; i < view_units.size(); i++) {
        view_units[i].render();
    }

}

int main()
{
    Rengine engine(WIN_W, WIN_H, "Multi-object draw", FPS);
    engine->create_shader(0, "../shaders/basic_shading");
    engine->create_model(0, "");

    engine->create_view_unit(0, V, P);
    engine->get_view_unit(0)->register_shader(0);
    engine->get_shader_unit(0)->create_model_unit(0, 

}
