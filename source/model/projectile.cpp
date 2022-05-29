#include "model/projectile.h"
#include "constants.h"
#include "model/handler.h"

namespace Tanks::model {

Projectile::Projectile(int left,
                       int top,
                       Direction direction,
                       GameModel &model,
                       int entityId)
    : MovableEntity(left,
                    top,
                    entityId,
                    std::make_unique<ProjectileHandler>(model, *this),
                    direction) {
}

EntityType Projectile::getType() const {
    return EntityType::BULLET;
}

int Projectile::getStrength() const {
    return 1;  // TODO make constant
}

int Projectile::getWidth() const {
    return BULLET_SIZE;
}

int Projectile::getHeight() const {
    return BULLET_SIZE;
}

int Projectile::getSpeed() const {
    return DEFAULT_BULLET_SPEED;
}

bool Projectile::isTankPassable() const {
    return true;
}
}  // namespace Tanks::model
