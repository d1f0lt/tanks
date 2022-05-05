cmake_minimum_required(VERSION 3.16)
project(tanks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/)
message("CMake runtime output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


SET(GCC_WARNINGS "-Wall -Wextra")
#SET(SANITIZERS "-fsanitize=address,undefined,leak,vptr")
#SET(GCC_OPTIMIZE "-Og -g -ftrapv") # Latest is integer overflow protection
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
        menu_tests
        menu_tests.cpp
        source/menu.cpp
        source/menu_items.cpp
        source/menu_controller.cpp
)

SET(COMPILER_FLAGS_FOR_MENU_TEST "-D MENU_TEST")
target_compile_options(menu_tests PRIVATE ${COMPILER_FLAGS_FOR_MENU_TEST})

add_executable(
        tanks
        source/main.cpp
        source/game.cpp
        source/menu_controller.cpp
        source/game_controller.cpp
        source/menu.cpp
        source/menu_items.cpp
        source/main_menu.cpp
        source/new_game_menu.cpp
        source/levels_menu.cpp
        source/pause.cpp
        source/game_environment.cpp
        source/view/game_view.cpp
        source/view/tank_view.cpp
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
        source/model/handler.cpp
)


target_link_libraries(model_tests sfml-graphics)
target_link_libraries(menu_tests sfml-graphics)
target_link_libraries(tanks sfml-graphics)
