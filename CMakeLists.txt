cmake_minimum_required(VERSION 3.16)
project(tanks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/)


find_package(SFML 2.5 COMPONENTS graphics audio)

add_executable(model_tests model_tests.cpp
        source/model/blocks.cpp
        source/model/entity.cpp
        source/model/game_model.cpp
        source/model/projectile.cpp
        source/model/tank.cpp
        source/model/grouped_entities.cpp
        source/model/game_map.cpp
        source/model/entity_holder.cpp
        source/model/foreground_entity.cpp source/model/event.cpp headers/model/event.h source/model/movable_entity.cpp headers/model/movable_entity.h source/model/events_queue.cpp headers/model/events_queue.h)


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
        source/pause.cpp
)


target_link_libraries(model_tests sfml-graphics)
target_link_libraries(tanks sfml-graphics)
