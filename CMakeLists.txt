cmake_minimum_required(VERSION 3.21)
project(tanks)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
find_package(SFML 2.5 COMPONENTS graphics audio)

add_executable(tanks main.cpp
        src/blocks.cpp
        src/entity.cpp
        src/game_model.cpp
        src/projectile.cpp
        src/tank.cpp
        src/grouped_entities.cpp
        src/game_map.cpp
        src/entity_holder.cpp include/entity_holder.h)

target_link_libraries(tanks sfml-graphics sfml-audio)
