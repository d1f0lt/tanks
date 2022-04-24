cmake_minimum_required(VERSION 3.16)
project(tanks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/)
message("CMake runtime output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


SET(GCC_WARNINGS "-Wall -Wextra")
SET(SANITIZERS "-fsanitize=address,undefined,leak,vptr")
SET(GCC_OPTIMIZE "-Og -g -ftrapv") # Latest is integer overflow protection
#SET(GCC_OPTIMIZE "-O2 -DNDEBUG") # RELEASE
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_WARNINGS} ${SANITIZERS} ${GCC_OPTIMIZE}")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(model_tests model_tests.cpp
        source/model/blocks.cpp
        source/model/entity.cpp
        source/model/game_model.cpp
        source/model/projectile.cpp
        source/model/tank.cpp
        source/model/grouped_entities.cpp
        source/model/game_map.cpp
        source/model/entity_holder.cpp
        source/model/foreground_entity.cpp
        source/model/movable_entity.cpp
        source/model/handler.cpp)


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
        source/pause.cpp
        source/main_menu.cpp
        source/game_environment.cpp
        source/model/handler.cpp)


target_link_libraries(model_tests sfml-graphics)
target_link_libraries(tanks sfml-graphics)
