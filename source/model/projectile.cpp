#include "model/projectile.h"
#include "constants.h"

namespace Tanks::model {
Projectile::Projectile(int left_,
                       int top_,
                       Direction direction_,
                       std::unique_ptr<BasicHandler> handler_)
    : MovableEntity(left_,
                    top_,
                    direction_,
                    BULLET_SPEED,
                    std::move(handler_)) {
}

Projectile::Projectile(int left_,
                       int top_,
                       Direction direction_,
                       GameModel &model_)
    : MovableEntity(left_,
                    top_,
                    direction_,
                    BULLET_SPEED,
                    std::make_unique<MovableHandler>(model_, *this)) {
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
}  // namespace Tanks::model
