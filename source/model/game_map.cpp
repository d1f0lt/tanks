#include "model/game_map.h"
#include <cassert>
#include <fstream>
#include <string>
#include "constants.h"

namespace Tanks::model {
int GameMap::getWidth() const {
    assert(!map.empty());
    return static_cast<int>(map[0].size());
}

int GameMap::getHeight() const {
    return static_cast<int>(map.size());
}

Entity &GameMap::getEntityByCoords(int col, int row) {
    assert(0 <= col && col < getHeight() && 0 <= row && row < getWidth());
    assert(map[row][col] != nullptr);

    return *map[row][col];
}

void GameMap::insert(Entity &entity) {
    if (map[entity.getTop()][entity.getLeft()] == &entity) {
        return;
    }

    for (int y = entity.getTop(); y < entity.getTop() + entity.getHeight();
         y++) {
        for (int x = entity.getLeft(); x < entity.getLeft() + entity.getWidth();
             x++) {
            map[y][x] = &entity;
        }
    }
}

void GameMap::eraseByCoords(int col, int row) {
    erase(getEntityByCoords(col, row));
}

void GameMap::erase(Entity &entity) {
    for (int y = entity.getTop(); y < entity.getTop() + entity.getHeight();
         y++) {
        for (int x = entity.getLeft(); x < entity.getLeft() + entity.getWidth();
             x++) {
            // TODO: default block
            map[y][x] = nullptr;
        }
    }
}

GameMap::GameMap(int width, int height)
    : map(height * TILE_SIZE,
          std::vector<Entity *>(width * TILE_SIZE, nullptr)) {
}

}  // namespace Tanks::model
