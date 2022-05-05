#include "model/tank.h"
#include <cassert>
#include <queue>
#include "constants.h"
#include "model/playable-tank.h"

namespace Tanks::model {
Tank::Tank(int left_,
           int top_,
           Direction direction_,
           std::unique_ptr<BasicHandler> handler_)
    : MovableEntity(left_,
                    top_,
                    std::move(handler_),
                    direction_,
                    DEFAULT_TANK_SPEED) {
}

bool Tank::canPass(const Entity &other) const {
    return other.isTankPassable();
}

void Tank::shoot() {
    dynamic_cast<TankHandler &>(*handler.get()).shoot();
}

int Tank::getWidth() const {
    return TANK_SIZE;
}

int Tank::getHeight() const {
    return TANK_SIZE;
}

int Tank::getStrength() const {
    return 0;
}

Tank::Tank(int left,
           int top,
           int id,
           std::unique_ptr<BasicHandler> handler,
           Direction direction,
           int speed)
    : MovableEntity(left, top, id, std::move(handler), direction, speed) {
}
EntityType Tank::getType() const {
    return EntityType::LEFT_DOWN_CORNER;
}

PlayableTank::PlayableTank(int left_,
                           int top_,
                           Direction direction_,
                           std::unique_ptr<BasicHandler> handler_)
    : Tank(left_, top_, direction_, std::move(handler_)) {
}

PlayableTank::PlayableTank(int left, int top, Direction dir, GameModel &model)
    : Tank(left, top, dir, std::make_unique<TankHandler>(model, *this)) {
}

EntityType PlayableTank::getType() const {
    return EntityType::PLAYABLE_TANK;
}

PlayableTank::PlayableTank(int left,
                           int top,
                           int id,
                           std::unique_ptr<BasicHandler> handler,
                           Direction direction,
                           int speed)
    : Tank(left, top, id, std::move(handler), direction, speed) {
}
void PlayableTank::move(Direction direction) {
    move(direction, getSpeed());
}

BotTank::BotTank(int left_,
                 int top_,
                 Direction direction_,
                 std::unique_ptr<BasicHandler> handler_)
    : Tank(left_, top_, direction_, std::move(handler_)) {
}
}  // namespace Tanks::model
