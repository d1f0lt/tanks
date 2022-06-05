#include "model/movable_entity.h"
#include "model/handler.h"

namespace Tanks::model {
void MovableEntity::move(Direction direction, int speed) {
    dynamic_cast<MovableHandler &>(getHandler()).move(direction, speed);
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
std::vector<const Entity *> MovableEntity::look(Direction direction) {
    return dynamic_cast<MovableHandler &>(getHandler()).look(direction);
}
}  // namespace Tanks::model
