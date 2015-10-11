CC = g++

CFLAGS = -pg -g -Wall -lGL -lGLEW -lSDL2 -lSDL2_image -lm -ljson-c `pkg-config --cflags --libs pangocairo` 

all: shark.bin toblerone_fish.bin \
	multi_object_draw.cpp ../boaler/boaler.cpp \
	../boaler/boaler.hpp \
	../simple_obj_reader/bin_obj_reader.cpp \
	../simple_obj_reader/bin_obj_reader.hpp \
	../shader_compiler/shader_compiler.cpp \
	../shader_compiler/shader_compiler.hpp \
	../utils/utils.cpp ../utils/utils.hpp \
	../InputProcessor/gen_input_processor.hpp \
	../cameras/fp_camera.cpp ../cameras/fp_camera.hpp \
	../billboard/billboard.cpp ../billboard/billboard.hpp \
	../text_texture/text_texture.cpp ../text_texture/text_texture.hpp \
	mo_billboard.cpp mo_billboard.hpp \
	../utils/base_eng/base_eng.cpp \
	../utils/base_eng/base_eng.hpp \
	mo_eng.hpp mo_eng.cpp
	$(CC) multi_object_draw.cpp ../boaler/boaler.cpp \
		../simple_obj_reader/bin_obj_reader.cpp  \
		../shader_compiler/shader_compiler.cpp \
		../utils/utils.cpp \
		../cameras/fp_camera.cpp \
		../billboard/billboard.cpp \
		../text_texture/text_texture.cpp \
		mo_billboard.cpp \
		../utils/base_eng/base_eng.cpp \
		mo_eng.cpp \
	   $(CFLAGS) -o multi_object_draw

simple_obj_reader: ../simple_obj_reader/simple_obj_reader.cpp
	$(CC) ../simple_obj_reader/simple_obj_reader.cpp \
		$(CFLAGS) -o ../simple_obj_reader/simple_obj_reader

shark.bin: simple_obj_reader
	../simple_obj_reader/simple_obj_reader \
		resources/fishbowl/shark/shark.obj > \
	resources/fishbowl/shark/shark.bin

toblerone_fish.bin: simple_obj_reader
	../simple_obj_reader/simple_obj_reader \
		resources/fishbowl/toblerone_fish/toblerone_fish.obj > \
	resources/fishbowl/toblerone_fish/toblerone_fish.bin

clean:
	rm -f *.o multi_object_draw
