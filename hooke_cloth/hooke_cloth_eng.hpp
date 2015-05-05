#ifndef HOOKE_CLOTH_ENG_H
#define HOOKE_CLOTH_ENG_H

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Cloth;
class ClothPt;

class ClothPt {
    public:
    ClothPt *right;
    ClothPt *above;
    Cloth *cloth;

    glm::vec3 *r;
    glm::vec3 v;
    glm::vec3 a;

    void calc_force();
    void iterate(double dt);
    ClothPt(Cloth *cloth, glm::vec3 *r);
};

class Cloth {
    public:
    unsigned int pts_w;
    unsigned int pts_h;
    std::vector<ClothPt> cloth_pts;
    double hooke_constant;
    double equil_length;

    void calc_force();
    void iterate(double dt);
    void load_into_buffer(GLuint buffer);
    Cloth(
        unsigned int pts_w,
        unsigned int pts_h,
        std::vector<glm::vec3> &points,
        double hooke_constant,
        double equil_length);

    private:
    void init_points(std::vector<glm::vec3> &points);
};

#endif
