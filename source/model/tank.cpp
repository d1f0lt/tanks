#include "model/tank.h"
#include <queue>
#include "constants.h"
#include "model/playable-tank.h"

namespace Tanks::model {
Tank::Tank(int left,
           int top,
           int entityId,
           std::unique_ptr<TankHandler> handler,
           Direction direction,
           int speed)
    : MovableEntity(left, top, entityId, std::move(handler), direction),
      speed_(speed) {
}

int Tank::getWidth() const {
    return TANK_SIZE;
}

int Tank::getHeight() const {
    return TANK_SIZE;
}

int Tank::getStrength() const {
    return 1;
}

EntityType Tank::getType() const {
    return EntityType::LEFT_DOWN_CORNER;
}

int Tank::getSpeed() const {
    return speed_;
}

bool Tank::canPass(const Entity &other) const {
    return other.isTankPassable();
}

void Tank::shoot() {
    dynamic_cast<TankHandler &>(getHandler()).shoot();
}

PlayableTank::PlayableTank(int left,
                           int top,
                           int entityId,
                           Direction direction,
                           GameModel &model)
    : Tank(left,
           top,
           entityId,
           std::make_unique<TankHandler>(model, *this),
           direction,
           DEFAULT_TANK_SPEED) {
}

EntityType PlayableTank::getType() const {
    return EntityType::PLAYABLE_TANK;
}

PlayableTank::PlayableTank(int left,
                           int top,
                           int entityId,
                           std::unique_ptr<TankHandler> handler,
                           Direction direction)
    : Tank(left,
           top,
           entityId,
           std::move(handler),
           direction,
           DEFAULT_TANK_SPEED) {
}
void PlayableTank::move(Direction direction) {
    move(direction, getSpeed());
}

}  // namespace Tanks::model
