#include "model/tank.h"
#include <queue>
#include "constants.h"
#include "model/tank_handler.h"

namespace Tanks::model {
Tank::Tank(int left,
           int top,
           DecrId entityId,
           std::unique_ptr<TankHandler> handler,
           Direction direction,
           int speed,
           int reloadTicks)
    : MovableEntity(left, top, entityId, std::move(handler), direction),
      speed_(speed),
      reloadTicks_(reloadTicks) {
}

int MediumTank::getWidth() const {
    return Tanks::TANK_SIZE;
}

MediumTank::MediumTank(int left,
                       int top,
                       DecrId entityId,
                       const TankHandlerCreator &handlerCreator,
                       Direction direction,
                       int speed)
    : Tank(left,
           top,
           entityId,
           handlerCreator.createTankHandler(*this),
           direction,
           speed,
           DEFAULT_RELOAD_TICKS) {
}

int MediumTank::getHeight() const {
    return TANK_SIZE;
}

int MediumTank::getStrength() const {
    return 1;
}

EntityType MediumTank::getType() const {
    return EntityType::MEDIUM_TANK;
}

int Tank::getSpeed() const {
    return speed_;
}

int Tank::getReloadTicks() const {
    return reloadTicks_;
}
}  // namespace Tanks::model
