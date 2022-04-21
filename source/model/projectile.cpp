#include "model/projectile.h"

namespace Tanks::model {
Projectile::Projectile(int col, int row, GameMap &map_)
    : ForegroundEntity(col, row, 5, 5, EntityType::BULLET, map_) {
}

void Projectile::move() {
    move_(dir);
}
}  // namespace Tanks::model
