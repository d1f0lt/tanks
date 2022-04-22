#include "model/projectile.h"

namespace Tanks::model {
Projectile::Projectile(int col, int row, Direction dir, GameMap &map_)
    : MovableEntity(col, row, 5, 5, EntityType::BULLET, dir, map_) {
    // TODO : width and height consts
}

}  // namespace Tanks::model
