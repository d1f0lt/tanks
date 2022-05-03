#include "model/movable_entity.h"
#include "model/handler.h"

namespace Tanks::model {
void MovableEntity::move(Direction dir) {
    handler->move(dir);
}

MovableEntity::MovableEntity(int left,
                             int right,
                             Direction direction,
                             int speed,
                             std::unique_ptr<BasicHandler> handler_)
    : ForegroundEntity(left, right, std::move(handler_)),
      direction(direction),
      speed(speed) {
}

Direction MovableEntity::getDirection() const {
    return direction;
}

void MovableEntity::setDirection(Direction dir) {
    direction = dir;
}

int MovableEntity::getSpeed() const {
    return speed;
}
}  // namespace Tanks::model
