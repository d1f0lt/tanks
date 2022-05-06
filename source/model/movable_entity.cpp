#include "model/movable_entity.h"
#include "model/handler.h"

namespace Tanks::model {
void MovableEntity::move(Direction dir, int speed) {
    dynamic_cast<MovableHandler &>(*handler_).move(dir, speed);
}

MovableEntity::MovableEntity(int left,
                             int right,
                             std::unique_ptr<MovableHandler> handler,
                             Direction direction)
    : ForegroundEntity(left, right, std::move(handler)), direction_(direction) {
}

Direction MovableEntity::getDirection() const {
    return direction_;
}

void MovableEntity::setDirection(Direction dir) {
    direction_ = dir;
}

MovableEntity::MovableEntity(int left,
                             int top,
                             int id,
                             std::unique_ptr<MovableHandler> handler,
                             Direction direction)
    : ForegroundEntity(left, top, id, std::move(handler)),
      direction_(direction) {
}
}  // namespace Tanks::model
