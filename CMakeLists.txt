cmake_minimum_required(VERSION 3.16)
project(tanks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/)
message("CMake runtime output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(
        tanks
        source/main.cpp
        source/tank.cpp
        source/player.cpp
        source/bullet.cpp
        source/movable_object.cpp
        source/game.cpp
        source/movable_object_view.cpp
        source/map.cpp
        source/controller.cpp
        source/menu.cpp
        source/menu_items.cpp
        source/pause.cpp
        source/main_menu.cpp
        source/game_environment.cpp
        source/new_game_menu.cpp
        source/levels_menu.cpp
)

target_link_libraries(tanks sfml-graphics sfml-audio)