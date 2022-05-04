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
    assert(0 <= col && col < getWidth() && 0 <= row && row < getHeight());
    assert(map[row][col] != nullptr);

    return *map[row][col];
}

void GameMap::insert(Entity &entity) {
    for (int row = entity.getTop(); row < entity.getTop() + entity.getHeight();
         row++) {
        for (int col = entity.getLeft();
             col < entity.getLeft() + entity.getWidth(); col++) {
            map[row][col] = &entity;
        }
    }
}

void GameMap::eraseByCoords(int col, int row) {
    erase(getEntityByCoords(col, row));
}

void GameMap::erase(Entity &entity) {
    for (int row = entity.getTop(); row < entity.getTop() + entity.getHeight();
         row++) {
        for (int col = entity.getLeft();
             col < entity.getLeft() + entity.getWidth(); col++) {
            // TODO: default block
            map[row][col] = nullptr;
        }
    }
}

GameMap::GameMap(int width, int height)
    : map(height * TILE_SIZE,
          std::vector<Entity *>(width * TILE_SIZE, nullptr)) {
}

}  // namespace Tanks::model
