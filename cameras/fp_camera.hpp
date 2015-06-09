#ifndef FP_CAMERA_H
#define FP_CAMERA_H

#include "../utils/base_eng/base_eng.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

enum cam_mv_dn {
    CAM_MV_NONE = 0,
    CAM_MV_UP = 1,
    CAM_MV_DOWN = 1<<1,
    CAM_MV_LEFT = 1<<2,
    CAM_MV_RIGHT = 1<<3,
    CAM_MV_FORWARD = 1<<4 };

typedef unsigned int (*get_direction_fn)(const BaseEng &);

class FpCamera {
    public:
        FpCamera(
            BaseEng &engine,
            double speed,
            double omega,
            const glm::vec3 &position,
            get_direction_fn get_direction);

        BaseEng &engine;

        glm::mat4 get_V();
        void compute_direction();

    private:
        double speed_frame_unit;
        double omega_frame_unit;
        glm::vec3 position;
        get_direction_fn get_direction;
        double phi;
        double theta;
};
#endif
