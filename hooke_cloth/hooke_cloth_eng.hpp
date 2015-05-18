#ifndef HOOKE_CLOTH_ENG_H
#define HOOKE_CLOTH_ENG_H

#include "../utils/base_eng/base_eng.hpp"

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
    void iterate();
    ClothPt();
    ClothPt(Cloth *cloth, glm::vec3 *r);
};

class Cloth {
    public:
    BaseEng &eng;
    unsigned int pts_w;
    unsigned int pts_h;
    std::vector<glm::vec3> vertices;
    std::vector<ClothPt> cloth_pts;

    double g_frame_unit;
    double hooke_constant_frame_unit;
    double damping_constant_frame_unit;

    double equil_length;

    void calc_force();
    void iterate();
    void load_into_buffer(GLuint buffer);
    Cloth(
        BaseEng &eng,
        unsigned int pts_w,
        unsigned int pts_h,
        std::vector<glm::vec3> &points,
        double g,
        double hooke_constant,
        double damping_constant,
        double equil_length);
    void add_fixed_pt(unsigned int x, unsigned int y);
    private:
    void init_points(std::vector<glm::vec3> &points);
    std::vector<unsigned int> fixed_pts;
};

#endif
