#include <stdio.h>

enum {
    width = 4,
    height = 4,
    texture_total_width = 512,
    texture_total_height = 512,
    texture_width = 300,
    texture_height = 300 
};

int main()
{
    for (unsigned int j = 0; j < height + 1; j++) {
        for (unsigned int i = 0; i < width + 1; i++) {
            fprintf(stdout, "v %d %d 0 \n", i, j);
        }
    }

    fprintf(stdout, "vt 0 0\n"
            "vt %f 0\n"
            "vt 0 %f\n"
            "vt %f %f\n",
            (float)texture_width / (float)texture_total_width,
            (float)texture_height / (float)texture_total_height,
            (float)texture_width / (float)texture_total_width,
            (float)texture_height / (float)texture_total_height);
    fprintf(stdout, "vn 0 0 1\n");

    for (unsigned int j = 0; j < height; j++) {
        for (unsigned int i = 0; i < width; i++) {
            fprintf(
                stdout,
                "f %d/3/1 %d/4/1 %d/2/1\n"
                    "f %d/3/1 %d/2/1 %d/1/1\n",
                1 + i + (width + 1) * j,
                1 + i + 1 + (width + 1) * j,
                1 + i + 1 + (width + 1) * (j + 1),
                1 + i + (width + 1) * j,
                1 + i + 1 + (width + 1) * (j + 1),
                1 + i + (width + 1) * (j + 1)
            );
        }
    }
}
