# define makefile variables
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib


# Executable "hello" depends on the files hello.o and run.o.
hello: bin/Main.o bin/Watchable.o bin/Movie.o bin/Episode.o bin/TvSeries.o bin/ConfigMovieReader.o bin/ConfigSeriesReader.o bin/Session.o bin/User.o
	@echo 'Linking...'
	$(CC) -o bin/splflix bin/Main.o bin/Watchable.o bin/Movie.o bin/Episode.o bin/TvSeries.o bin/ConfigMovieReader.o bin/ConfigSeriesReader.o bin/Session.o bin/User.o $(LFLAGS)

# Object code (*.o file) depends on the source and header files
bin/Main.o: src/Main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp

# Depends on the source and header files 
bin/Watchable.o: include/Watchable.h src/Watchable.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Watchable.o src/Watchable.cpp

bin/Movie.o: include/Watchable.h src/Movie.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Movie.o src/Movie.cpp

bin/Episode.o: include/Watchable.h src/Episode.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Episode.o src/Episode.cpp

bin/TvSeries.o: include/TvSeries.h src/TvSeries.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/TvSeries.o src/TvSeries.cpp

bin/ConfigMovieReader.o: include/ConfigMovieReader.h src/ConfigMovieReader.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/ConfigMovieReader.o src/ConfigMovieReader.cpp

bin/ConfigSeriesReader.o: include/ConfigSeriesReader.h src/ConfigSeriesReader.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/ConfigSeriesReader.o src/ConfigSeriesReader.cpp

bin/Session.o: include/Session.h src/Session.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Session.o src/Session.cpp

bin/User.o: include/User.h src/User.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/User.o src/User.cpp

#Clean the build directory
clean: 
	rm -f bin/*
