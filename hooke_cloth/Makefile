CC = g++

CFLAGS = -pg -g -Wall -lGL -lGLEW -lSDL2 -lSDL2_image -lm

all: cloth.bin cursor.bin \
	hooke_cloth.cpp hooke_cloth_eng.cpp \
	hooke_cloth_eng.hpp \
	../simple_obj_reader/bin_obj_reader.cpp \
	../simple_obj_reader/bin_obj_reader.hpp \
	../shader_compiler/shader_compiler.cpp \
	../shader_compiler/shader_compiler.hpp \
	../utils/utils.cpp ../utils/utils.hpp \
	../InputProcessor/InputProcessor.cpp \
	../InputProcessor/InputProcessor.hpp \
	../utils/base_eng/base_eng.cpp \
	../utils/base_eng/base_eng.hpp \
	drawable.hpp drawable.cpp
	$(CC) hooke_cloth.cpp hooke_cloth_eng.cpp \
		../simple_obj_reader/bin_obj_reader.cpp  \
		../shader_compiler/shader_compiler.cpp \
		../utils/utils.cpp \
		../InputProcessor/InputProcessor.cpp \
		../utils/base_eng/base_eng.cpp \
		drawable.cpp \
	   $(CFLAGS) -o hooke_cloth

cloth.obj: gen_cloth.cpp
	$(CC) gen_cloth.cpp -o gen_cloth
	./gen_cloth > cloth.obj

cloth.bin: ../simple_obj_reader/simple_obj_reader.cpp cloth.obj
	$(CC) ../simple_obj_reader/simple_obj_reader.cpp  \
	   $(CFLAGS) -o ../simple_obj_reader/simple_obj_reader
	../simple_obj_reader/simple_obj_reader cloth.obj > cloth.bin

cursor.bin: ../simple_obj_reader/simple_obj_reader.cpp
	$(CC)  ../simple_obj_reader/simple_obj_reader.cpp  \
	   $(CFLAGS) -o ../simple_obj_reader/simple_obj_reader
	../simple_obj_reader/simple_obj_reader cursor.obj > cursor.bin

clean:
	rm -f *.o menge_sponge gen_menge_grid
