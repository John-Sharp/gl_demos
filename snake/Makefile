CC = gcc

CFLAGS = -pg -g -Wall -lGL -lGLEW -lSDL2 -lSDL2_image

all: main.c engine.c engine.h 
	$(CC) engine.c main.c $(CFLAGS) -o snake

clean:
	rm -f *.o
