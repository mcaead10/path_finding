cmake_minimum_required(VERSION 3.2)
project(tp3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES tp3a.cpp)
add_executable(tp3 ${SOURCE_FILES} carte.h carte.cpp coordonnee.h coordonnee.cpp heure.h heure.cpp reseau.h reseau.cpp)