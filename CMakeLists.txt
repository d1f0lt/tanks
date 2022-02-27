cmake_minimum_required(VERSION 3.21)
project(tanks)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/model)
find_package(SFML 2.5 COMPONENTS graphics audio)

add_executable(tanks main.cpp
        source/model/blocks.cpp
        source/model/entity.cpp
        source/model/game_model.cpp
        source/model/projectile.cpp
        source/model/tank.cpp
        source/model/grouped_entities.cpp
        source/model/game_map.cpp
        source/model/entity_holder.cpp)

target_link_libraries(tanks sfml-graphics sfml-audio)
