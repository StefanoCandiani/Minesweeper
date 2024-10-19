cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(Project3)

set(CMAKE_CXX_STANDARD 11)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)
add_executable(Project3 main.cpp introcard.cpp gamewindow.cpp tile.cpp leaderboard.cpp)
target_link_libraries(Project3 sfml-system sfml-window sfml-graphics sfml-audio)