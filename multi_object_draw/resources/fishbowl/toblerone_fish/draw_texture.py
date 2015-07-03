import subprocess
import os

points = [
        [0, 0.866],
        [0, 0.2],
        [0.5, 0.2],
        [0, -0.2],
        [0.5, -0.2],
        [0, -0.866],
        [0.8464, -0.4],
        [0.3464, - 1.066]
        ]

shapes = [
        (0, 1, 2),
        (1, 2, 4, 3),
        (3, 4, 5),
        (4, 6, 7, 5)
        ]

target_dims = (1024, 1024)

def get_min_max(shapes):
    min_coord = [0, 0]
    max_coord = [0, 0]

    for shape in shapes:
        for p in shape:
            pt = points[p]
            if pt[0] < min_coord[0]:
                min_coord[0] = pt[0]
            if pt[1] < min_coord[1]:
                min_coord[1] = pt[1]


            if pt[0] > max_coord[0]:
                max_coord[0] = pt[0]
            if pt[1] > max_coord[1]:
                max_coord[1] = pt[1]

    return [min_coord, max_coord]

def get_scale_factor(bounding_box):
    sf_x = target_dims[0] / bounding_box[0]
    sf_y = target_dims[1] / bounding_box[1]

    return min([sf_x, sf_y])

def translate_and_scale_and_flip(points, vector, sf):
    for pt in points:
        for i in range(2):
            pt[i] += vector[i]
            pt[i] *= sf
        pt[1] = target_dims[1] - pt[1]

def draw_and_save_shapes():
    preamble = """
 convert -size %dx%d xc:white -fill white -stroke black -draw \"push graphic-context
 fill 'white' stroke 'black' %s pop graphic-context\" net.svg""" % (
            target_dims[0], target_dims[1], "%s")
    closed_path_string = "path 'M %s Z'"
    shape_strings = []
    for shape in shapes:
        path_contents = []
        for p in shape:
            pt = points[p]
            path_contents.append(",".join([str(int(p)) for p in pt]))

        shape_strings.append(closed_path_string % " ".join(path_contents))


    convert_command = preamble % " ".join(shape_strings)
    print convert_command
    os.system(convert_command)
    # subprocess.call(["convert", convert_command])


min_coord, max_coord = get_min_max(shapes)
sf = get_scale_factor([max_coord[0] - min_coord[0], max_coord[1] - min_coord[1]])

translate_and_scale_and_flip(points, [-1 * m for m in min_coord], sf)

draw_and_save_shapes()
