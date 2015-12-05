CC = gcc

CFLAGS = -pg -g -Wall -lGL -lGLEW -lSDL2 -lSDL2_image

all: simple.vertexshader simple.fragmentshader main.c engine.c engine.h input_processor.c input_processor.h
	xxd -i < simple.vertexshader > simple_vertexshader.h
	echo ', 0' >> simple_vertexshader.h
	xxd -i < simple.fragmentshader > simple_fragmentshader.h
	echo ', 0' >> simple_fragmentshader.h
	$(CC) engine.c input_processor.c main.c $(CFLAGS) -o snake

clean:
	rm -f *.o
