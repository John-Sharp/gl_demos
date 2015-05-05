CC = g++

CFLAGS = -pg -g -Wall -lGL -lGLEW -lSDL2 -lSDL2_image -lm

all: hooke_cloth.cpp hooke_cloth_eng.cpp \
	hooke_cloth_eng.hpp \
	../simple_obj_reader/bin_obj_reader.cpp \
	../simple_obj_reader/bin_obj_reader.hpp \
	../shader_compiler/shader_compiler.cpp \
	../shader_compiler/shader_compiler.hpp \
	../utils/utils.cpp ../utils/utils.hpp \
	../InputProcessor/InputProcessor.cpp \
	../InputProcessor/InputProcessor.hpp
	$(CC) hooke_cloth.cpp hooke_cloth_eng.cpp \
		../simple_obj_reader/bin_obj_reader.cpp  \
		../shader_compiler/shader_compiler.cpp \
		../utils/utils.cpp \
		../InputProcessor/InputProcessor.cpp \
	   $(CFLAGS) -o hooke_cloth

clean:
	rm -f *.o menge_sponge gen_menge_grid
