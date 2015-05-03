CC = g++

CFLAGS = -pg -g -Wall -lGL -lGLEW -lSDL2 -lSDL2_image -lm

all: menge_sponge.cpp ../simple_obj_reader/bin_obj_reader.cpp \
	../simple_obj_reader/bin_obj_reader.hpp \
	../shader_compiler/shader_compiler.cpp \
	../shader_compiler/shader_compiler.hpp \
	../InputProcessor/InputProcessor.cpp \
	../InputProcessor/InputProcessor.hpp \
    read_menge_grid.cpp read_menge_grid.hpp	
	$(CC) menge_sponge.cpp ../simple_obj_reader/bin_obj_reader.cpp  \
		../shader_compiler/shader_compiler.cpp \
		../InputProcessor/InputProcessor.cpp \
		read_menge_grid.cpp \
	   $(CFLAGS) -o menge_sponge

gen_menge_grid: gen_menge_grid.cpp
	$(CC) gen_menge_grid.cpp -o gen_menge_grid

clean:
	rm -f *.o menge_sponge gen_menge_grid
