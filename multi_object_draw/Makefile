CC = g++

CFLAGS = -pg -g -Wall -lGL -lGLEW -lSDL2 -lSDL2_image -lm

all: multi_object_draw.cpp boaler.cpp \
	boaler.hpp \
	../simple_obj_reader/bin_obj_reader.cpp \
	../simple_obj_reader/bin_obj_reader.hpp \
	../shader_compiler/shader_compiler.cpp \
	../shader_compiler/shader_compiler.hpp \
	../utils/utils.cpp ../utils/utils.hpp \
	../InputProcessor/InputProcessor.cpp \
	../InputProcessor/InputProcessor.hpp \
	../utils/base_eng/base_eng.cpp \
	../utils/base_eng/base_eng.hpp
	$(CC) multi_object_draw.cpp boaler.cpp \
		../simple_obj_reader/bin_obj_reader.cpp  \
		../shader_compiler/shader_compiler.cpp \
		../utils/utils.cpp \
		../InputProcessor/InputProcessor.cpp \
		../utils/base_eng/base_eng.cpp \
	   $(CFLAGS) -o multi_object_draw

clean:
	rm -f *.o multi_object_draw
