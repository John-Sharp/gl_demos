#include "InputProcessor.hpp"
#include <iostream>

InputProcessor::InputProcessor(
    double speed,
    double omega,
    const glm::vec3 &position) :
        actions(0),
        theta(M_PI),
        phi(0)
{
    this->speed = speed;
    this->omega = omega;    
    this->position = position;
}

enum {
    UP_PRESSED = 1,
    DOWN_PRESSED = 1<<1,
    LEFT_PRESSED = 1<<2,
    RIGHT_PRESSED = 1<<3,
    FORWARD_PRESSED = 1<<4
};

void InputProcessor::process_event(SDL_Event *event)
{
    switch(event->key.keysym.sym) {
        case SDLK_UP:
            if (event->key.type == SDL_KEYDOWN) {
                this->actions |= UP_PRESSED;
            } else {
                this->actions ^= UP_PRESSED;
            }
            break;
        case SDLK_DOWN:
            if (event->key.type == SDL_KEYDOWN) {
                this->actions |= DOWN_PRESSED;
            } else {
                this->actions ^= DOWN_PRESSED;
            }
            break;
        case SDLK_LEFT:
            if (event->key.type == SDL_KEYDOWN) {
                this->actions |= LEFT_PRESSED;
            } else {
                this->actions ^= LEFT_PRESSED;
            }
            break;
        case SDLK_RIGHT:
            if (event->key.type == SDL_KEYDOWN) {
                this->actions |= RIGHT_PRESSED;
            } else {
                this->actions ^= RIGHT_PRESSED;
            }
            break;
        case SDLK_a:
            if (event->key.type == SDL_KEYDOWN) {
                this->actions |= FORWARD_PRESSED;
            } else {
                this->actions ^= FORWARD_PRESSED;
            }
        default:
            break;
    }
}

void InputProcessor::compute_direction(double dt)
{
    double dtheta = 0, dphi = 0, dr = 0;
    if (this->actions == 0) {
        return;
    }
    if (this->actions & UP_PRESSED) { 
        dphi += 1; 
    }
    if (this->actions & DOWN_PRESSED) {
        dphi -= 1;
    }
    if (this->actions & LEFT_PRESSED) {
        dtheta += 1;
    }
    if (this->actions & RIGHT_PRESSED) {
        dtheta -= 1;
    }
    if (this->actions & FORWARD_PRESSED) {
        dr += 1;
    }

    this->position.x += dr * sin(this->theta) * cos(this->phi) * this->speed * dt;
    this->position.y += dr * sin(this->phi) * this->speed * dt;
    this->position.z += dr * cos(this->theta) * cos(this->phi) * this->speed * dt;
    // this->position.z += dr * this->speed * dt;

    std::cout << this->position.x << ',' << this->position.y << ',' << this->position.z << '\n';
    if (dtheta == 0 && dphi == 0) {
        return;
    }

    double mag = sqrt(dtheta * dtheta + dphi * dphi);
    this->theta += dtheta/mag * this->omega * dt;
    this->phi += dphi/mag * this->omega * dt;
}

glm::mat4 InputProcessor::get_view_mat()
{
    glm::vec3 looking_dn;
    glm::vec3 up_dn;

    looking_dn.x = sin(this->theta) * cos(this->phi);
    looking_dn.y = sin(this->phi);
    looking_dn.z = cos(this->theta) * cos(this->phi);

    up_dn.x = -sin(this->theta) * sin(this->phi);
    up_dn.y = cos(this->phi);
    up_dn.z = -cos(this->theta) * sin(this->phi);

    // std::cout << this->position.x << ',' << this->position.y << ',' << this->position.z << '\n';

    return glm::lookAt(
            this->position,
            this->position + looking_dn,
            up_dn);
}

glm::mat4 InputProcessor::get_proj_mat()
{
    return glm::perspective(44.9f, 4.0f / 3.0f, 0.1f, 100.0f);
}
