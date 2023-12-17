cmake_minimum_required(VERSION 3.5.0)
project(SnakeNN VERSION 2.1.0 LANGUAGES C CXX)

set(SFML_DIR "scr/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window REQUIRED)

add_executable(SnakeNN scr/Source.cpp)

target_link_libraries(SnakeNN
    sfml-graphics
    sfml-window
    sfml-system
)
