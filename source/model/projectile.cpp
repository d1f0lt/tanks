#include "model/projectile.h"
#include "constants.h"

namespace Tanks::model {
Projectile::Projectile(int left_,
                       int top_,
                       Direction direction_,
                       std::unique_ptr<BasicHandler> handler_)
    : MovableEntity(left_,
                    top_,
                    BULLET_SIZE,
                    BULLET_SIZE,
                    EntityType::BULLET,
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
                    BULLET_SIZE,
                    BULLET_SIZE,
                    EntityType::BULLET,
                    direction_,
                    BULLET_SPEED,
                    std::make_unique<MovableHandler>(model_, *this)) {
}
}  // namespace Tanks::model
