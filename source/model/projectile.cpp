#include "model/projectile.h"
#include "constants.h"

namespace Tanks::model {
Projectile::Projectile(int left,
                       int top,
                       Direction direction,
                       std::unique_ptr<ProjectileHandler> handler)
    : MovableEntity(left, top, std::move(handler), direction) {
}

bool Projectile::canPass(const Entity &other) const {
    return other.isBulletPassable();
}

Projectile::Projectile(int left, int top, Direction direction, GameModel &model)
    : MovableEntity(left,
                    top,
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
    return BULLET_SPEED;
}

bool Projectile::isTankPassable() const {
    return true;
}
}  // namespace Tanks::model
