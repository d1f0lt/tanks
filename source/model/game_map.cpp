#include "model/game_map.h"
#include <cassert>
#include "constants.h"

namespace Tanks::model {
int GameMap::getWidth() const {
    assert(!map_.empty());
    return static_cast<int>(map_[0].size());
}

int GameMap::getHeight() const {
    return static_cast<int>(map_.size());
}

Entity &GameMap::getEntityByCoords(int col, int row) const {
    assert(0 <= col && col < getWidth() && 0 <= row && row < getHeight());
    assert(map_[row][col] != nullptr);

    return *map_[row][col];
}

void GameMap::insert(Entity &entity) {
    for (int row = entity.getTop(); row < entity.getTop() + entity.getHeight();
         row++) {
        for (int col = entity.getLeft();
             col < entity.getLeft() + entity.getWidth(); col++) {
            map_[row][col] = &entity;
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
            map_[row][col] = nullptr;
        }
    }
}

GameMap::GameMap(size_t width, size_t height)
    : map_(height * TILE_SIZE, std::vector<Entity *>(width * TILE_SIZE)) {
}

}  // namespace Tanks::model
