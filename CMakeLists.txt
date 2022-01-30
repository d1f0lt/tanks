cmake_minimum_required(VERSION 3.16)
project(tanks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
find_package(SFML 2.5 COMPONENTS graphics audio)

add_executable(
        tanks
        source/cpp/main.cpp
        source/cpp/tank.cpp
        source/cpp/player.cpp
        source/headers/tank.h
        source/headers/player.h
        source/headers/constants.h
)

target_link_libraries(tanks sfml-graphics sfml-audio)