CC = g++

CFLAGS = -pg -g -Wall -lGL

simple_obj_reader: simple_obj_reader.cpp 
	$(CC) simple_obj_reader.cpp  \
	   $(CFLAGS) -o simple_obj_reader

bin_obj_reader: bin_obj_reader.cpp
	$(CC) bin_obj_reader.cpp \
		-shared -fPIC -o libbinobjreader.so

clean:
	rm -f *.o simple_obj_reader bin_obj_reader
