//
// Created by ilma4 on 2/18/22.
//

#include "game_map.h"
#include <cassert>

namespace tanks::model {
int GameMap::getWidth() const {
    assert(!map.empty());
    return static_cast<int>(map[0].size());
}

int GameMap::getHeight() const {
    return static_cast<int>(map.size());
}

Entity *GameMap::getEntityByCoords(const sf::Vector2<int> &coords) {
    assert(0 < coords.y && coords.y < getHeight() && 0 < coords.x &&
           coords.x < getWidth());
    return map[coords.y][coords.x];
}

void GameMap::addEntity(Entity *entity) {
    // TODO: check it
    for (int y = entity->getBottom(); y <= entity->getTop(); y++) {
        for (int x = entity->getLeft(); x <= entity->getRight(); x++) {
            map[y][x] = entity;
        }
    }
}

void GameMap::removeEntityByCoords(const sf::Vector2<int> &coords) {
    removeEntity(getEntityByCoords(coords));
}

void GameMap::removeEntity(Entity *entity) {
    for (int y = entity->getBottom(); y <= entity->getTop(); y++) {
        for (int x = entity->getLeft(); x <= entity->getRight(); x++) {
            // TODO: default block
            map[y][x] = nullptr;
        }
    }
}
}  // namespace tanks::model
