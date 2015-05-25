#ifndef DRAWABLE_HPP_
#define DRAWABLE_HPP_

#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdlib.h>
#include <stdio.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Drawable
{
    private:
        // std::vector and buffer object for storing the element
        // array data
        std::vector<GLuint> element_array;
        GLuint element_array_bo;
        
        // std::vector, buffer object and shader attribute variable
        // for storing the position vertex data
        std::vector<glm::vec3> positions;
        GLuint position_bo;
        GLint position_attrib;

        // std::vector with size = positions.size() where each element
        // gives the index of the ordered_positions
        // vertex that is the same as the positions vertex corresponding
        // to that element
        std::vector<GLuint> element_array_positions_ordered_positions_map;

        // std::vector, buffer object and shader attribute variable
        // for storing the texture coordinate data
        std::vector<glm::vec2> uvs;
        GLuint uv_bo;
        GLint uv_attrib;

        // std::vector, buffer object and shader attribute variable
        // for storing the normals coordinate data
        std::vector<glm::vec3> normals;
        GLuint normal_bo;
        GLint normal_attrib;

        // Reference to this Drawable's shader program
        GLuint program_id;

        // Regerence to this Drawable's vertex array object
        GLuint vao;

        // Reference to this Drawable's texture unit and texture
        GLenum texture_unit;
        GLuint texture_id;

        // References to shader's M V and P matrices
        GLuint M_id; 
        GLuint V_id; 
        GLuint P_id; 

    public:
        Drawable(
            const char *bin_model_fname,
            GLuint program_id,
            GLuint vao,
            GLenum texture_unit,
            GLuint texture_id);

        ~Drawable();

        // Succession of methods that bind and load
        // the corresponding vector of attributes into
        // a buffer object
        void load_element_array_bo();
        void load_position_bo();
        void load_normal_bo();
        void load_uv_bo();
        // loads both the vertex and the normal buffer 
        // objects
        void load_position_normal_bo();

        // std::vector of positions ordered in the same way that they
        // are declared in the *.obj file that contains the model
        std::vector<glm::vec3> ordered_positions;

        glm::mat4 M;
        glm::mat4 V;
        glm::mat4 P;


        // set drawable's V and P matrices
        void set_V(glm::mat4 new_V);
        void set_P(glm::mat4 new_V);

        // updates the positions and normals based on the values
        // contained in new_ordered_positions
        void update_vertices(
            std::vector<glm::vec3> &new_ordered_positions);

        void draw();
};

#endif
