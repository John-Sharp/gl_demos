import sys

fname = sys.argv[2]
scale_factor = float(sys.argv[1])

lines = open(fname).readlines()

for l in lines:
    ls = l.split()
    if ls[0] == "v":
        for i in range(1, 4):
            ls[i] = str(float(ls[i]) * scale_factor)
    print " ".join(ls)
