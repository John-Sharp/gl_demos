#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL2/SDL.h>

class InputProcessor
{
    private:
        int actions;
        double theta;
        double phi;
        glm::vec3 position;
        double speed;
        double omega;

    public:
        glm::mat4 get_proj_mat();
        glm::mat4 get_view_mat();
        void compute_direction(double dt);
        void process_dvent(SDL_Event *event);
        InputProcessor(double speed, double omega);
};
#endif
