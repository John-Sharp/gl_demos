CC = g++

CFLAGS = -pg -g -Wall -lGL -lGLEW -lSDL2 -lSDL2_image -lm

all: menge_sponge.cpp ../simple_obj_reader/bin_obj_reader.cpp \
	../simple_obj_reader/bin_obj_reader.hpp \
	../shader_compiler/shader_compiler.cpp \
	../shader_compiler/shader_compiler.hpp \
	../InputProcessor/InputProcessor.cpp \
	../InputProcessor/InputProcessor.hpp
	$(CC) menge_sponge.cpp ../simple_obj_reader/bin_obj_reader.cpp  \
		../shader_compiler/shader_compiler.cpp \
		../InputProcessor/InputProcessor.cpp \
	   $(CFLAGS) -o menge_sponge

clean:
	rm -f *.o menge_sponge
