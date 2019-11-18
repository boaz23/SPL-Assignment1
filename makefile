# define makefile variables
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib


# Executable "hello" depends on the files hello.o and run.o.
hello: bin/Main.o
	@echo 'Building target: hello by Invoking C++ Linker'
	$(CC) -o bin/splflix bin/Main.o $(LFLAGS)

# Object code (*.o file) depends on the source and header files
bin/Main.o: src/Main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp

# Depends on the source and header files 
bin/include/json.o: src/run.cpp  include/helloWorld.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/run.o src/run.cpp

#Clean the build directory
clean: 
	rm -f bin/*
