#include "model/bonus.h"
#include "constants.h"

namespace Tanks::model {
Bonus::Bonus(int left,
             int top,
             int entityId,
             std::unique_ptr<BonusHandler> handler)
    : ForegroundEntity(left, top, entityId, std::move(handler)) {
}

int Bonus::getWidth() const {
    return BONUS_SIZE;
}
int Bonus::getHeight() const {
    return BONUS_SIZE;
}

bool Bonus::canPass(const Entity &other) const {
    return other.isTankPassable();
}

WalkOnWater::WalkOnWater(int left, int top, int entityId, GameModel &model)
    : Bonus(left,
            top,
            entityId,
            std::make_unique<WalkOnWaterHandler>(model, *this)) {
}

EntityType WalkOnWater::getType() const {
    return EntityType::WALK_ON_WATER_BONUS;
}
}  // namespace Tanks::model
