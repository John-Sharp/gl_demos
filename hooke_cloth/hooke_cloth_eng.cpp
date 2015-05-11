#include "hooke_cloth_eng.hpp"

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>

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


    for (unsigned int i = 0; i < this->cloth_pts.size(); i++) {
        fprintf(stderr, "cloth pt %d: (%f, %f, %f)\n",i, this->cloth_pts[i].r->x, this->cloth_pts[i].r->y, this->cloth_pts[i].r->z);
        if (this->cloth_pts[i].right != NULL) {
            fprintf(stderr, "cloth pt %d right: (%f, %f, %f)\n",i, this->cloth_pts[i].right->r->x, this->cloth_pts[i].right->r->y, this->cloth_pts[i].right->r->z);
        }

        if (this->cloth_pts[i].above != NULL) {
            fprintf(stderr, "cloth pt %d above: (%f, %f, %f)\n",i, this->cloth_pts[i].above->r->x, this->cloth_pts[i].above->r->y, this->cloth_pts[i].above->r->z);
        }

    }

    fprintf(stderr, "GOT HERE!!\n");
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

    for (unsigned int j = 0; j < h; j++) {
        for (unsigned int i = 0; i < w; i++) {
            int index = w * j + i;
            this->vertices[index] = pts[index];
            this->cloth_pts.push_back(ClothPt(this, &this->vertices[index]));
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

    // At the moment have two hard-coded fixed cloth points
    // (in the top left and top right of the cloth)
    this->cloth_pts[(h - 1) * w + w - 1].a = glm::vec3(0,0,0);
    this->cloth_pts[(h - 1) * w].a = glm::vec3(0,0,0);
}

void Cloth::iterate(double dt)
{
    unsigned int w = this->pts_w;
    unsigned int h = this->pts_h;

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
    double x_o = this->cloth->equil_length;

    glm::vec3 a_right = glm::vec3(0.0f, 0.0f, 0.0f);
    if (this->right) {
        double x_right = glm::length(*(this->right->r) - *(this->r));
        glm::vec3 d_right = glm::normalize(*(this->right->r) - *(this->r));
        a_right = (GLfloat)(k *  (x_right - x_o)) * d_right;
        this->a += a_right;
        this->right->a -= a_right;
    }

    if (this->above) {
        double x_above = glm::length(*(this->above->r) - *(this->r));
        glm::vec3 d_above = glm::normalize(*(this->above->r) - *(this->r));
        glm::vec3 a_above = (GLfloat)(k * (x_above - x_o)) * d_above;
        this->a += a_above;
        this->above->a -= a_above;
    }

    // Addition of gravity
    this->a += glm::vec3(0,-1,0);
}

void ClothPt::iterate(double dt)
{
    *(this->r) += v * (GLfloat)dt;
    this->v += a * (GLfloat)dt;
    this->a = glm::vec3(0,0,0);
}

