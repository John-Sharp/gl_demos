#include "hooke_cloth_eng.hpp"

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

Cloth::Cloth(
    unsigned int pts_w,
    unsigned int pts_h,
    std::vector<glm::vec3> &pts,
    double hooke_constant,
    double equil_length)
{
    this->pts_w = pts_w;
    this->pts_h = pts_h;
    this->init_points(pts);
    this->hooke_constant = hooke_constant;
    this->equil_length = equil_length;
}

void Cloth::init_points(std::vector<glm::vec3> &pts)
{
    int w = this->pts_w;
    int h = this->pts_h;

    if (pts.size() != w * h) {
        fprint(stderr, "Not enough points for specified width and height!\n");
        exit(1);
    }

    for (unsigned int j = 0; j < h; j++) {
        for (unsigned int i = 0; i < w; i++) {
            int index = w * j + i;
            this->cloth_pts.push_back(ClothPt(this, &pts[index]));
            if (i != 0) {
                this->cloth_pts[index - 1]->right = &this->cloth_pts[index]; 
            }
            if (j != 0) {
                this->cloth_pts[index - w]->above = &this->cloth_pts[index];
            }
        }
    }
}

Cloth::calc_force()
{
    int w = this->pts_w;
    int h = this->pts_h;

    for (unsigned int j = 0; j < h; j++) {
        for (unsigned int i = 0; i < w; i++) {
            int index = w * j + i;
            this->cloth_pts[index].calc_force();
        }
    }

    this->cloth_pts[(h - 1) * w + w - 1]->a = glm::vec3(0,0,0);
    this->cloth_pts[(h - 1) * w]->a = glm::vec3(0,0,0);
}

Cloth::iterate(double dt)
{
    int w = this->pts_w;
    int h = this->pts_h;

    for (unsigned int j = 0; j < h; j++) {
        for (unsigned int i = 0; i < w; i++) {
            int index = w * j + i;
            this->cloth_pts[index].iterate(dt);
        }
    }
}

ClothPt::ClothPt(Cloth *cloth, glm::vec3 *r)
{
    this->cloth = cloth;
    this->r = r;
    this->v = glm::vec3(0,0,0);
    this->a = glm::vec3(0,0,0);
    this->right = NULL;
    this->above = NULL;
}

void ClothPt::calc_force()
{
    double k = this->cloth->hooke_constant;
    double x_right = glm::length(*(this->right->r) - *(this->r));
    double x_o = this->cloth->equil_length;

    glm::vec3 d_right = normalize(*(this->right->r) - *(this->r));

    glm::vec3 a_right = k *  (x_right - x_o) * d_right;

    this->a += a_right;
    if (this->right) {
        this->right->a -= a_right;
    }

    double x_above = glm::length(*(this->above->r) - *(this->r));
    glm::vec3 d_above = normalize(*(this->above->r) - *(this->r));
    glm::vec3 a_above = k * (x_above - x_o) * d_above;

    this->a += a_above;
    if (this->above) {
        this->above->a -= a_above;
    }

    this->a += glm::vec3(0,-1,0);
}

void ClothPt::iterate(double dt)
{
    this->r += v * dt;
    this->v += a * dt;
    this->a = 0;
}

