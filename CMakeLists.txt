cmake_minimum_required(VERSION 3.15)
project(ass1)

set(CMAKE_CXX_STANDARD 11)

add_executable(ass1 src/Main.cpp src/Action.cpp src/test.cpp src/ConfigMovieReader.cpp src/ConfigSeriesReader.cpp src/TvSeries.cpp src/Session.cpp src/Watchable.cpp src/Movie.cpp src/Episode.cpp)