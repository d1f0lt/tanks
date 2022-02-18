//
// Created by ilma4 on 2/18/22.
//
#pragma once

#include "entity.h"

namespace tanks::model {
class Tank : public Entity {
public:
    void move(Direction dir);

private:
    Direction direction;
    static constexpr int WIDTH = 3;
    static constexpr int HEIGHT = WIDTH;
    std::array<std::array<Entity, WIDTH>, HEIGHT> background;
};
}  // namespace tanks::model
