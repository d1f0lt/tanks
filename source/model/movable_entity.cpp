#include "model/movable_entity.h"

namespace Tanks::model {
void MovableEntity::move(Direction dir) {
    setDirection(dir);
    restoreBackground();
    switch (dir) {
        case Direction::UP:
            setTop(getTop() - 1);
            break;
        case Direction::LEFT:
            setLeft(getLeft() - 1);
            break;
        case Direction::DOWN:
            setTop(getTop() + 1);
            break;
        case Direction::RIGHT:
            setLeft(getLeft() + 1);
            break;
    }
    setBackground();
}

MovableEntity::MovableEntity(int col,
                             int row,
                             int width,
                             int height,
                             EntityType type,
                             Direction dir,
                             GameMap &map)
    : ForegroundEntity(col, row, width, height, type, map), direction(dir) {
}

Direction MovableEntity::getDirection() const {
    return direction;
}

void MovableEntity::setDirection(Direction dir) {
    direction = dir;
}
}  // namespace Tanks::model
