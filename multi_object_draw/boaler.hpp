#ifndef BOALER_H
#define BOALER_H

#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

typedef class BoalerVSLink BoalerVSLink;
typedef class BoalerVSLModelUnitLink BoalerVSLModelUnitLink;

// BoalerModel
class BoalerModel
{
    public:
        BoalerModel(
            const char *bin_model_fname);

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
        std::vector<glm::vec3> normals; GLuint normal_bo;
};

class BoalerModelUnit
{
    public:
        BoalerModelUnit(
            glm::mat4 M,
            unsigned int texture_unit_index,
            GLuint texture_id,
            BoalerModel &model);


        // Model unit's model matrix
        glm::mat4 M;

        unsigned int texture_unit_index;
        GLuint texture_id;

        // Pointer to the linked model
        BoalerModel *model;
};

class BoalerShaderUnit
{
    public:
        BoalerShaderUnit(GLuint program_id);

        // Reference to shader unit's program
        GLuint program_id;

        // References to model unit's shader's position, uv and
        // normal attributes
        GLint position_attr;
        GLint uv_attr;
        GLint normal_attr;

        // References to shader program's M, V and P
        // uniforms
        GLuint M_unfm; 
        GLuint V_unfm;
        GLuint P_unfm;

        GLuint texture_sampler_unfm;
};

class BoalerViewUnit
{
    public:
        BoalerViewUnit(
            glm::mat4 V,
            glm::mat4 P);

        // vector to list of view unit's shader units
        std::vector<BoalerVSLink *> shader_links;

        // view unit's view and projection matrices
        glm::mat4 V;
        glm::mat4 P;

        void render();

    private:

};

class BoalerVSLink
{
    public:
        BoalerVSLink(
            BoalerViewUnit &view_unit,
            BoalerShaderUnit &shader_unit);

        // References to the link's view and shader units
        BoalerViewUnit *view_unit;
        BoalerShaderUnit *shader_unit;

        // Handler method that is called and links the ViewUnit's
        // member variables with the shader's uniforms
        virtual void shader_unit_linker();

        // list of VS link - model unit links that should be rendered with this shader and view unit
        std::vector<BoalerVSLModelUnitLink *> model_unit_links;
};

class BoalerVSLModelUnitLink
{
    public:
        BoalerVSLModelUnitLink(
            BoalerVSLink &vs_link,
            BoalerModelUnit &model_unit);
        void update_model_unit(BoalerModelUnit &model_unit);

        // Pointers to the linked VS link and model unit
        BoalerVSLink *vs_link;
        BoalerModelUnit *model_unit;
        
        // Reference to this vertex array object that links this
        // model unit with the shader referenced through this link
        GLuint vao;

        virtual void model_unit_linker();

        void render();
};

class BoalerEng
{
    public:
        std::vector<BoalerModel *> models;
        std::vector<BoalerModelUnit *> model_units;
        std::vector<BoalerShaderUnit *> shader_units;
        std::vector<BoalerViewUnit *> view_units;

        void reg_view_unit(BoalerViewUnit *vu);
        void reg_shader_unit(BoalerShaderUnit *su);
        void reg_model(BoalerModel *m);
        void reg_model_unit(BoalerModelUnit *m);

        void render();
};

#endif
