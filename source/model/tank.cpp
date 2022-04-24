#include "model/tank.h"
#include <cassert>
#include <queue>
#include "constants.h"
#include "model/playable-tank.h"

namespace Tanks::model {
Tank::Tank(int left_,
           int top_,
           EntityType type_,
           Direction direction_,
           std::unique_ptr<BasicHandler> handler_)
    : MovableEntity(left_,
                    top_,
                    TANK_SIZE,
                    TANK_SIZE,
                    type_,
                    direction_,
                    TANK_SPEED,
                    std::move(handler_)) {
}

Tank::Tank(int left_,
           int top_,
           EntityType type_,
           Direction direction_,
           GameModel &model_)
    : MovableEntity(left_,
                    top_,
                    TANK_SIZE,
                    TANK_SIZE,
                    type_,
                    direction_,
                    TANK_SPEED,
                    std::make_unique<TankHandler>(model_, *this)) {
}

void Tank::shoot() {
    handler->shoot();
}

PlayableTank::PlayableTank(int left_,
                           int top_,
                           Direction direction_,
                           std::unique_ptr<BasicHandler> handler_)
    : Tank(left_,
           top_,
           EntityType::PLAYABLE_TANK,
           direction_,
           std::move(handler_)) {
}

PlayableTank::PlayableTank(int left, int top, Direction dir, GameModel &model)
    : Tank(left, top, EntityType::PLAYABLE_TANK, dir, model) {
}

BotTank::BotTank(int left_,
                 int top_,
                 Direction direction_,
                 std::unique_ptr<BasicHandler> handler_)
    : Tank(left_, top_, EntityType::BOT_TANK, direction_, std::move(handler_)) {
}
}  // namespace Tanks::model
