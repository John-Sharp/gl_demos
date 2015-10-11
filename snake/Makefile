CC = gcc

CFLAGS = -pg -g -Wall -lGL -lGLEW -lSDL2 -lSDL2_image

all: main.c engine.c engine.h input_processor.c input_processor.h
	$(CC) engine.c input_processor.c main.c $(CFLAGS) -o snake

clean:
	rm -f *.o
