#include "model/movable_entity.h"
#include "model/handler.h"

namespace Tanks::model {
void MovableEntity::move(Direction dir) {
    handler->move(dir);
}

MovableEntity::MovableEntity(int left,
                             int right,
                             std::unique_ptr<BasicHandler> handler_,
                             Direction direction,
                             int speed)
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
MovableEntity::MovableEntity(int left,
                             int top,
                             int id,
                             std::unique_ptr<BasicHandler> handler,
                             Direction direction,
                             int speed)
    : ForegroundEntity(left, top, id, std::move(handler)),
      direction(direction),
      speed(speed) {
}
}  // namespace Tanks::model
