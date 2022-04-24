#include "model/tank.h"
#include <cassert>
#include <queue>
#include "model/playable-tank.h"

namespace Tanks::model {
Tank::Tank(int left,
           int top,
           EntityType type_,
           Direction dir,
           std::unique_ptr<BasicHandler> handler_)
    : MovableEntity(left,
                    top,
                    TANK_SIZE,
                    TANK_SIZE,
                    type_,
                    dir,
                    std::move(handler_)) {
}

PlayableTank::PlayableTank(int left,
                           int top,
                           Direction dir,
                           std::unique_ptr<BasicHandler> handler_)
    : Tank(left, top, EntityType::PLAYABLE_TANK, dir, std::move(handler_)) {
}

void Tank::shoot() {
    handler->shoot(*this);
}

BotTank::BotTank(int left,
                 int top,
                 Direction dir,
                 std::unique_ptr<BasicHandler> handler_)
    : Tank(left, top, EntityType::BOT_TANK, dir, std::move(handler_)) {
}
}  // namespace Tanks::model
