cmake_minimum_required(VERSION 3.16)
project(tanks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/)
message("CMake runtime output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

SET(GCC_WARNINGS "-Wall -Wextra -pthread")
#SET(SANITIZERS "-fsanitize=address,undefined,leak,vptr,enum,null,shift,vla-bound")
#SET(SANITIZERS "-fsanitize=address,undefined,leak,vptr,enum,null,shift,vla-bound")
#SET(SANITIZERS "-fsanitize=address,undefined,leak,vptr")
#SET(SANITIZERS "-fsanitize=address,undefined")
#SET(GCC_OPTIMIZE "-ggdb -g3 -ftrapv -D_GLIBCXX_DEBUG") # Latest is integer overflow protection
#SET(GCC_OPTIMIZE "-Og -ggdb -g ")# Latest is integer overflow protection
#SET(GCC_OPTIMIZE "-O2 -mtune=native") # RELEASE
#SET(GCC_OPTIMIZE "-O2 -DNDEBUG -mtune=native") # RELEASE
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_WARNINGS} ${SANITIZERS} ${GCC_OPTIMIZE}")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(model_tests
        model_tests.cpp
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
        source/model/event.cpp
        source/model/server_game_model.cpp
        source/model/playable_tank_handler.cpp
        source/model/event_executor.cpp
        source/model/client_game_model.cpp
        source/model/spawners.cpp
        source/model/network_utils.cpp
        source/model/bonus.cpp
        source/model/tank_handler.cpp
        )

add_executable(
        menu_tests
        menu_tests.cpp
        source/menu/menu.cpp
        source/menu/menu_items.cpp
        source/menu/menu_controller.cpp
)

SET(COMPILER_FLAGS_FOR_MENU_TEST "-D MENU_TEST")
target_compile_options(menu_tests PRIVATE ${COMPILER_FLAGS_FOR_MENU_TEST})

add_executable(
        tanks
        source/main.cpp
        source/menu/menu_controller.cpp
        source/menu/menu.cpp
        source/menu/menu_items.cpp
        source/menu/main_menu.cpp
        source/menu/new_game_menu.cpp
        source/menu/levels_menu.cpp
        source/game.cpp
        source/game_controller.cpp
        source/game_environment.cpp
        source/pause.cpp
        source/view/game_view.cpp
        source/view/tank_view.cpp
        source/view/bullets_view.cpp

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
        source/model/event.cpp
        source/model/server_game_model.cpp
        source/model/playable_tank_handler.cpp
        source/model/event_executor.cpp
        source/model/client_game_model.cpp
        source/model/spawners.cpp
        source/model/network_utils.cpp
        source/model/bonus.cpp
        source/model/tank_handler.cpp
        source/server.cpp source/server.h)

target_link_libraries(model_tests sfml-graphics)
target_link_libraries(menu_tests sfml-graphics)
target_link_libraries(tanks sfml-graphics)

