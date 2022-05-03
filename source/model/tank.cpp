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
                    direction_,
                    DEFAULT_TANK_SPEED,
                    std::move(handler_)) {
}

Tank::Tank(int left_, int top_, Direction direction_, GameModel &model_)
    : MovableEntity(left_,
                    top_,
                    direction_,
                    DEFAULT_TANK_SPEED,
                    std::make_unique<TankHandler>(model_, *this)) {
}

void Tank::shoot() {
    handler->shoot();
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

PlayableTank::PlayableTank(int left_,
                           int top_,
                           Direction direction_,
                           std::unique_ptr<BasicHandler> handler_)
    : Tank(left_, top_, direction_, std::move(handler_)) {
}

PlayableTank::PlayableTank(int left, int top, Direction dir, GameModel &model)
    : Tank(left, top, dir, model) {
}

EntityType PlayableTank::getType() const {
    return EntityType::PLAYABLE_TANK;
}

BotTank::BotTank(int left_,
                 int top_,
                 Direction direction_,
                 std::unique_ptr<BasicHandler> handler_)
    : Tank(left_, top_, direction_, std::move(handler_)) {
}
}  // namespace Tanks::model
