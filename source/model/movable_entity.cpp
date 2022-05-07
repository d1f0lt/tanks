#include "model/movable_entity.h"
#include "model/handler.h"

namespace Tanks::model {
void MovableEntity::move(Direction direction, int speed) {
    dynamic_cast<MovableHandler &>(getHandler()).move(direction, speed);
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

void MovableEntity::setDirection(Direction direction) {
    direction_ = direction;
}

MovableEntity::MovableEntity(int left,
                             int top,
                             int entityId,
                             std::unique_ptr<MovableHandler> handler,
                             Direction direction)
    : ForegroundEntity(left, top, entityId, std::move(handler)),
      direction_(direction) {
}
}  // namespace Tanks::model
