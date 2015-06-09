#include "fp_camera.hpp"

FpCamera::FpCamera(
    BaseEng &engine,
    double speed,
    double omega,
    const glm::vec3 &position,
    get_direction_fn get_direction) :
        engine(engine),
        speed_frame_unit(speed/engine.fps),
        omega_frame_unit(omega/engine.fps),
        position(position),
        get_direction(get_direction),
        phi(0),
        theta(M_PI)
{}

glm::mat4 FpCamera::get_V()
{
    glm::vec3 looking_dn;
    glm::vec3 up_dn;

    looking_dn.x = sin(theta) * cos(phi);
    looking_dn.y = sin(phi);
    looking_dn.z = cos(theta) * cos(phi);

    up_dn.x = -sin(theta) * sin(phi);
    up_dn.y = cos(phi);
    up_dn.z = -cos(theta) * sin(phi);

    return glm::lookAt(
            position,
            position + looking_dn,
            up_dn);
}

void FpCamera::compute_direction()
{
    double dtheta = 0, dphi = 0, dr = 0;
    unsigned int direction = get_direction(engine);

    if (direction == CAM_MV_NONE) {
        return;
    }
    if (direction & CAM_MV_UP) { 
        dphi += 1; 
    }
    if (direction & CAM_MV_DOWN) {
        dphi -= 1;
    }
    if (direction & CAM_MV_LEFT) {
        dtheta += 1;
    }
    if (direction & CAM_MV_RIGHT) {
        dtheta -= 1;
    }
    if (direction & CAM_MV_FORWARD) {
        dr += 1;
    }

    position.x += dr * sin(theta) * cos(phi) * speed_frame_unit;
    position.y += dr * sin(phi) * speed_frame_unit;
    position.z += dr * cos(theta) * cos(phi) * speed_frame_unit;

    if (dtheta == 0 && dphi == 0) {
        return;
    }

    double mag = sqrt(dtheta * dtheta + dphi * dphi);
    this->theta += dtheta/mag * omega_frame_unit;
    this->phi += dphi/mag * omega_frame_unit;
}
