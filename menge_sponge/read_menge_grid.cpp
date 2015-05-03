#include "read_menge_grid.hpp"

#include <stdio.h>

void bin_menge_grid_read(
        const char *path,
        std::vector<glm::vec3> &displacements
) {
    FILE *fp = fopen(path, "r");
    int displacements_array_size;

    fscanf(fp, "%d \n", &displacements_array_size);
    displacements.resize(displacements_array_size);
    fread(
        &displacements[0],
        sizeof(displacements[0]),
        displacements_array_size,
        fp);
    fclose(fp);
}
