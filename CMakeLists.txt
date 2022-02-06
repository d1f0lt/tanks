cmake_minimum_required(VERSION 3.16)
project(tanks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/)


find_package(SFML 2.5 COMPONENTS graphics audio)

add_executable(
        tanks
        source/main.cpp
        source/tank.cpp
        source/player.cpp
        source/bullet.cpp
        source/movable_object.cpp
        source/game.cpp
        source/movable_object_view.cpp)

target_link_libraries(tanks sfml-graphics sfml-audio)