#include "projectile.h"

namespace Tanks::model {
Projectile::Projectile(int col, int row, GameMap &map_)
    : ForegroundEntity(col, row, 5, 5, EntityType::BULLET, map_) {
}

void Projectile::nextState() {
    boom();
    move_(dir);
    boom();
}

void Projectile::boom() {
    bool isDead = false;
    for (int row = 0; row < getWidth(); row++) {
        for (int col = 0; col < getHeight(); col++) {
            if (!background[row][col]->isBulletPassable()) {
                isDead = true;
                // TODO
            }
        }
    }
    if (isDead) {
        // TODO
    }
}
}  // namespace Tanks::model
