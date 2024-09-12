cmake_minimum_required(VERSION 3.5.0)
project(SnakeNN VERSION 2.1.0 LANGUAGES C CXX)

add_executable(SnakeNN src/Source.cpp)

target_link_libraries(SnakeNN
    sfml-graphics
    sfml-window
    sfml-system
)
