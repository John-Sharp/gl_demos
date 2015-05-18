#include "hooke_cloth_eng.hpp"

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

Cloth::Cloth(
    BaseEng &eng,
    unsigned int pts_w,
    unsigned int pts_h,
    std::vector<glm::vec3> &pts,
    double g,
    double hooke_constant,
    double damping_constant,
    double equil_length) :
    eng(eng)
{
    this->pts_w = pts_w;
    this->pts_h = pts_h;
    this->init_points(pts);

    this->g_frame_unit = g / (eng.fps * eng.fps);
    this->hooke_constant_frame_unit = hooke_constant / (eng.fps * eng.fps);
    this->damping_constant_frame_unit = damping_constant / eng.fps;

    this->equil_length = equil_length;

    for (unsigned int i = 0; i < this->cloth_pts.size(); i++) {
        fprintf(stderr, "cloth pt %d: (%f, %f, %f)\n",i, this->cloth_pts[i].r->x, this->cloth_pts[i].r->y, this->cloth_pts[i].r->z);
        if (this->cloth_pts[i].right != NULL) {
            fprintf(stderr, "cloth pt %d right: (%f, %f, %f)\n",i, this->cloth_pts[i].right->r->x, this->cloth_pts[i].right->r->y, this->cloth_pts[i].right->r->z);
        }

        if (this->cloth_pts[i].above != NULL) {
            fprintf(stderr, "cloth pt %d above: (%f, %f, %f)\n",i, this->cloth_pts[i].above->r->x, this->cloth_pts[i].above->r->y, this->cloth_pts[i].above->r->z);
        }

    }
}

void Cloth::init_points(std::vector<glm::vec3> &pts)
{
    unsigned int w = this->pts_w;
    unsigned int h = this->pts_h;

    if (pts.size() != w * h) {
        fprintf(stderr, "Not enough points for specified width and height!\n");
        exit(1);
    }

    this->vertices.resize(w * h);
    this->cloth_pts.resize(w * h);

    for (unsigned int j = 0; j < h; j++) {
        for (unsigned int i = 0; i < w; i++) {
            int index = w * j + i;
            this->vertices[index] = pts[index];
            this->cloth_pts[index] = ClothPt(this, &this->vertices[index]);
            if (i != 0) {
                this->cloth_pts[index - 1].right = &this->cloth_pts[index]; 
            }
            if (j != 0) {
                this->cloth_pts[index - w].above = &this->cloth_pts[index];
            }
        }
    }
}

void Cloth::calc_force()
{
    unsigned int w = this->pts_w;
    unsigned int h = this->pts_h;

    for (unsigned int j = 0; j < h; j++) {
        for (unsigned int i = 0; i < w; i++) {
            int index = w * j + i;
            this->cloth_pts[index].calc_force();
        }
    }

    // Fix the points that need to be fixed
    for (unsigned int i = 0; i < this->fixed_pts.size(); i++) {
        this->cloth_pts[this->fixed_pts[i]].a = glm::vec3(0,0,0);
    }
}

void Cloth::iterate()
{
    unsigned int w = this->pts_w;
    unsigned int h = this->pts_h;

    for (unsigned int j = 0; j < h; j++) {
        for (unsigned int i = 0; i < w; i++) {
            int index = w * j + i;
            this->cloth_pts[index].iterate();
        }
    }
}

void Cloth::add_fixed_pt(unsigned int x, unsigned int y)
{
    unsigned int pt_index = this->pts_h * y + x;

    if (pt_index + 1 > this->pts_w * this->pts_h) {
        return;
    }

    for (unsigned int i = 0; i < this->fixed_pts.size(); i++) {
        if (this->fixed_pts[i] == pt_index) {
            return;
        }
    }

    this->fixed_pts.push_back(pt_index);
}

ClothPt::ClothPt()
{
    this->cloth = NULL;
    this->r = NULL;
    this->v = glm::vec3(0,0,0);
    this->a = glm::vec3(0,0,0);
    this->right = NULL;
    this->above = NULL;
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
    double k = this->cloth->hooke_constant_frame_unit;
    float D = this->cloth->damping_constant_frame_unit;
    double g = this->cloth->g_frame_unit;
    double x_o = this->cloth->equil_length;

    glm::vec3 a_right = glm::vec3(0.0f, 0.0f, 0.0f);
    if (this->right) {
        double x_right = glm::length(*(this->right->r) - *(this->r));
        glm::vec3 d_right = glm::normalize(*(this->right->r) - *(this->r));
        a_right = (GLfloat)(k *  (x_right - x_o)) * d_right;
        this->a += a_right + (D * (this->right->v - this->v));
        this->right->a -= a_right  + (D * (this->right->v - this->v));
    }

    if (this->above) {
        double x_above = glm::length(*(this->above->r) - *(this->r));
        glm::vec3 d_above = glm::normalize(*(this->above->r) - *(this->r));
        glm::vec3 a_above = (GLfloat)(k * (x_above - x_o)) * d_above;
        this->a += a_above + (D * (this->above->v - this->v));
        this->above->a -= a_above + (D * (this->above->v - this->v));
    }

    // Addition of gravity
    this->a += glm::vec3(0,-g,0);
}

void ClothPt::iterate()
{
    *(this->r) += v;
    this->v += a;
    this->a = glm::vec3(0,0,0);
}

