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
           int speed,  // NOLINT(bugprone-easily-swappable-parameters)
           int reloadTicks,
           int bulletSpeed)
    : MovableEntity(left, top, entityId, std::move(handler), direction),
      speed_(speed),
      reloadTicks_(reloadTicks),
      bulletSpeed_(bulletSpeed) {
}

int MediumTank::getWidth() const {
    return Tanks::TANK_SIZE;
}

MediumTank::MediumTank(int left,
                       int top,
                       DecrId entityId,
                       const TankHandlerCreator &handlerCreator,
                       Direction direction,
                       int speed,
                       int reloadTicks,
                       int bulletSpeed)
    : Tank(left,
           top,
           entityId,
           handlerCreator.createTankHandler(*this),
           direction,
           speed,
           reloadTicks,
           bulletSpeed) {
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

bool Tank::isShootingThisTick() const {
    return dynamic_cast<TankHandler &>(getHandler()).isShootingThisTick();
}

bool Tank::hasBonus() const {
    return dynamic_cast<TankMovableOnWaterHandler *>(&getHandler()) != nullptr;
    // TODO TankWithBonusHandler
}

int Tank::getBulletSpeed() const {
    return bulletSpeed_;
}
}  // namespace Tanks::model
