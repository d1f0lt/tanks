#include "model/projectile.h"
#include "constants.h"
#include "model/handler.h"

namespace Tanks::model {

Projectile::Projectile(int left,
                       int top,
                       Direction direction,
                       GameModel &model,
                       IncrId entityId,
                       int speed,
                       int tank)
    : MovableEntity(left,
                    top,
                    entityId,
                    std::make_unique<ProjectileHandler>(model, *this),
                    direction),
      speed_(speed),
      tank_(tank) {
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
    return speed_;
}

bool Projectile::isTankPassable() const {
    return false;
}
int Projectile::getShooter() const {
    return tank_;
}
}  // namespace Tanks::model
