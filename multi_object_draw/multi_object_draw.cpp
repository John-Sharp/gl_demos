
#include "rengine.hpp"

enum { WIN_W = 800, WIN_H = 600, FPS = 100 };

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

class BoalerModelUnit
{
    public:
        // Reference to this Drawable's vertex array object
        GLuint vao;

        // Model unit's model matrix
        glm::mat4 M;

        // References to model unit's model matrix
        GLuint M_id; 

        // References to model unit's shader's position, uv and
        // normal attributes
        GLint position_attrib;
        GLint uv_attrib;
        GLint normal_attrib;

        // Reference to model unit's shader unit
        BoalerShaderUnit &shader_unit

    private:

}

class BoalerShaderUnit
{
    public:
        std::vector<BoalerModelUnit> model_units;

        // Reference to shader unit's program
        GLuint program_id;

        // References to shader program's V and P
        // uniforms
        GLuint V_id;
        GLuint P_id;

        // Reference to shader unit's engine
        BoalerEng &engine;

        // Handler method that gets called just prior
        // to the rendering of this shader unit
        void pre_render_handler();

    private:

}

class BoalerViewUnit
{
    public:
        // view unit's shader units
        std::vector<BoalerShaderUnit> shader_units;

        // view unit's view and projection matrices
        glm::mat4 V;
        glm::mat4 P;

        // Handler method that gets called just prior
        // to the rendering of this view unit
        void pre_view_handler();

        void pre_shader_unit_render_handler(BoalerShaderUnit &shader_unit);

        void render();


    private:

};

BoalerViewUnit::pre_shader_unit_render_handler(BoalerShaderUnit &shader_unit)
{
    return;
}

BoalerViewUnit::render()
{
    for (unsigned int i = 0; i < shader_units.size(); i++) {
        // set the shader's view and projection matrices
        GLuint V_id = shader_units[i].V_id;
        GLuint P_id = shader_units[i].P_id;
        glUniformMatrix4fv(V_id, 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(P_id, 1, GL_FALSE, &P[0][0]);
        pre_shader_unit_render_handler(shader_units[i]);
        shader_units[i].render();
    }
}

class BoalerEng
{
    public:
        std::vector<BoalerModel> models;
        std::vector<BoalerModelUnit> model_units;
        std::vector<BoalerSharderUnit> shader_units;
        std::vector<BoalerViewUnit> view_units;

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
