#pragma once
#ifndef TANKS_GAME_MAP_H
#define TANKS_GAME_MAP_H

#include "entity.h"

namespace tanks::model {
class GameMap {
public:
    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;

    [[nodiscard]] Entity *getEntityByCoords(const sf::Vector2<int> &coords);

    void addEntity(Entity *entity);

    void removeEntityByCoords(const sf::Vector2<int> &coords);
    void removeEntity(Entity *entity);

private:
    std::vector<std::vector<Entity *>> map;
};
}  // namespace tanks::model

#endif  // TANKS_GAME_MAP_H
