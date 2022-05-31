#include "model/bonus.h"
#include "constants.h"

namespace Tanks::model {
Bonus::Bonus(int left,
             int top,
             DecrId entityId,
             std::unique_ptr<BonusHandler> handler)
    : ForegroundEntity(left, top, entityId, std::move(handler)) {
}

int Bonus::getWidth() const {
    return BONUS_SIZE;
}

int Bonus::getHeight() const {
    return BONUS_SIZE;
}

int Bonus::getStrength() const {
    return 1;
}

void Bonus::apply(Tank &tank) {
    dynamic_cast<BonusHandler &>(getHandler()).apply(tank);
}

bool Bonus::isTankPassable() const {
    return true;
}

bool Bonus::isBulletPassable() const {
    return true;
}

WalkOnWater::WalkOnWater(int left, int top, DecrId entityId, GameModel &model)
    : Bonus(left,
            top,
            entityId,
            std::make_unique<WalkOnWaterHandler>(model, *this)) {
}

EntityType WalkOnWater::getType() const {
    return EntityType::WALK_ON_WATER_BONUS;
}

}  // namespace Tanks::model
