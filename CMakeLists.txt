cmake_minimum_required(VERSION 3.16)
project(tanks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/)
message("CMake runtime output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


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
        source/model/foreground_entity.cpp)

add_executable(
        tanks
        source/main.cpp
        source/game.cpp
        source/controller.cpp
        source/menu.cpp
        source/pause.cpp
        source/main_menu.cpp
        source/game_environment.cpp
        source/model/blocks.cpp
        source/model/entity.cpp
        source/model/game_model.cpp
        source/model/projectile.cpp
        source/model/tank.cpp
        source/model/grouped_entities.cpp
        source/model/game_map.cpp
        source/model/entity_holder.cpp
        source/model/foreground_entity.cpp
        source/view/game_view.cpp
        source/view/tank_view.cpp
)


target_link_libraries(model_tests sfml-graphics)
target_link_libraries(tanks sfml-graphics)
