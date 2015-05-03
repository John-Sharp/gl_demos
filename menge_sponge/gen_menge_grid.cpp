#include <vector>

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

enum {
    TOTAL_ITERATIONS = 3
};

bool is_in_sierpinski_carpet(unsigned int i, unsigned int j)
{
    while (i != 0 && j != 0) {
        if (i % 3 == 1 && j % 3 == 1) {
            return false;
        }
        i /= 3;
        j /= 3;
    }
    return true;
}

bool is_in_menge_sponge(unsigned int i, unsigned int j, unsigned int k)
{
    if (!is_in_sierpinski_carpet(i, j)) {
        return false;
    }
    if (!is_in_sierpinski_carpet(j, k)) {
        return false;
    }
    if (!is_in_sierpinski_carpet(k, i)) {
        return false;
    }
    return true;
}

int main()
{
    const int grid_side_len = pow(3, TOTAL_ITERATIONS);
    const int grid_size = (int)pow(grid_side_len, 2);
    int grid[grid_side_len][grid_side_len];
    std::vector<glm::vec3> instance_displacements;

    for (unsigned int i = 0; i < grid_side_len; i++){
        for (unsigned int j = 0; j < grid_side_len; j++) {
            for (unsigned int k = 0; k < grid_side_len; k++) {
                if (is_in_menge_sponge(i, j, k)) {
                    instance_displacements.push_back(
                        glm::vec3(i * 2.0f,j * 2.0f,k * -2.0f));
                }
            }
        }
    }

    fprintf(stdout, "%lu \n", instance_displacements.size());
    fwrite(
        &instance_displacements[0],
        sizeof(instance_displacements[0]),
        instance_displacements.size(),
        stdout);
}
