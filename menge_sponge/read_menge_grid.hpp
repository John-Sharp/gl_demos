#ifndef READ_MENGE_GRID_H
#define READ_MENGE_GRID_H

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

void bin_menge_grid_read(
        const char *path,
        std::vector<glm::vec3> &displacements
);

#endif
