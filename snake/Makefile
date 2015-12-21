CC = gcc

CFLAGS = -pg -g -Wall -lGL -lGLEW -lSDL2 -lSDL2_image

all: simple.vertexshader simple.fragmentshader main.c engine.c engine.h input_processor.c input_processor.h \
	sprite.c sprite.h decal.c decal.h actor.c actor.h snake_actor.c snake_actor.h background_actor.c background_actor.h \
	apple_actor.c apple_actor.h
	xxd -i < simple.vertexshader > simple_vertexshader.h
	echo ', 0' >> simple_vertexshader.h
	xxd -i < simple.fragmentshader > simple_fragmentshader.h
	echo ', 0' >> simple_fragmentshader.h
	$(CC) decal.c sprite.c engine.c input_processor.c actor.c snake_actor.c background_actor.c apple_actor.c main.c $(CFLAGS) -o snake

clean:
	rm -f *.o
