#ifndef TANKS_PROJECTILE_H
#define TANKS_PROJECTILE_H

#include "movable_entity.h"

namespace Tanks::model {

class Projectile : MovableEntity {
public:
    explicit Projectile(int col, int row, Direction dir, GameMap &map_);

private:
    Direction dir;
};
}  // namespace Tanks::model

#endif  // TANKS_PROJECTILE_H
