#ifndef TANKS_PROJECTILE_H
#define TANKS_PROJECTILE_H

#include "foreground_entity.h"

namespace Tanks::model {

class Projectile : ForegroundEntity {
public:
    explicit Projectile(int col, int row, GameMap &map_);

    // TODO: Make possible to use move() only for model
    void move();

private:
    Direction dir;
};
}  // namespace Tanks::model

#endif  // TANKS_PROJECTILE_H
