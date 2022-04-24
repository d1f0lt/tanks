#include "model/projectile.h"

namespace Tanks::model {
Projectile::Projectile(int col,
                       int row,
                       Direction dir,
                       std::unique_ptr<BasicHandler> handler_)
    : MovableEntity(col,
                    row,
                    5,
                    5,
                    EntityType::BULLET,
                    dir,
                    std::move(handler_)) {
    // TODO : width and height consts
}

}  // namespace Tanks::model
