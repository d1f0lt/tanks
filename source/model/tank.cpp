#include "model/tank.h"
#include <cassert>
#include <queue>
#include "constants.h"
#include "model/playable-tank.h"

namespace Tanks::model {
Tank::Tank(int left,
           int top,
           std::unique_ptr<TankHandler> handler,
           Direction direction,
           int speed)
    : MovableEntity(left, top, std::move(handler), direction), speed_(speed) {
}

bool Tank::canPass(const Entity &other) const {
    return other.isTankPassable();
}

void Tank::shoot() {
    dynamic_cast<TankHandler &>(*handler_.get()).shoot();
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

Tank::Tank(int left,
           int top,
           int id,
           std::unique_ptr<TankHandler> handler,
           Direction direction,
           int speed)
    : MovableEntity(left, top, id, std::move(handler), direction),
      speed_(speed) {
}
EntityType Tank::getType() const {
    return EntityType::LEFT_DOWN_CORNER;
}

int Tank::getSpeed() const {
    return speed_;
}

PlayableTank::PlayableTank(int left,
                           int top,
                           Direction direction,
                           std::unique_ptr<TankHandler> handler)
    : Tank(left, top, std::move(handler), direction, DEFAULT_TANK_SPEED) {
}

PlayableTank::PlayableTank(int left, int top, Direction dir, GameModel &model)
    : Tank(left,
           top,
           std::make_unique<TankHandler>(model, *this),
           dir,
           DEFAULT_TANK_SPEED) {
}

EntityType PlayableTank::getType() const {
    return EntityType::PLAYABLE_TANK;
}

PlayableTank::PlayableTank(int left,
                           int top,
                           int id,
                           std::unique_ptr<TankHandler> handler,
                           Direction direction)
    : Tank(left, top, id, std::move(handler), direction, DEFAULT_TANK_SPEED) {
}
void PlayableTank::move(Direction direction) {
    move(direction, getSpeed());
}

BotTank::BotTank(int left,
                 int top,
                 Direction direction,
                 std::unique_ptr<TankHandler> handler)
    : Tank(left, top, std::move(handler), direction, DEFAULT_TANK_SPEED) {
}
}  // namespace Tanks::model
