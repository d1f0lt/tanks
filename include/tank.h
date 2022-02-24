#pragma once
#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "entity.h"
#include "game_map.h"

namespace tanks::model {
class Tank : public Entity {
public:
    void move(Direction dir, const GameMap &map);

private:
    void updateBackground(const GameMap &map);

    Direction direction;
    static constexpr int WIDTH = 3;
    static constexpr int HEIGHT = WIDTH;
    std::array<std::array<Entity, WIDTH>, HEIGHT> background;
};
}  // namespace tanks::model

#endif  // TANKS_TANK_H
