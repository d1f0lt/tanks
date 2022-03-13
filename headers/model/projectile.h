#ifndef TANKS_PROJECTILE_H
#define TANKS_PROJECTILE_H

#include "foreground_entity.h"

namespace Tanks::model {

class Projectile : ForegroundEntity {
public:
    explicit Projectile(int col, int row, GameMap &map_);
    void nextState();

private:
    Direction dir;

    void boom();
};
}  // namespace Tanks::model

#endif  // TANKS_PROJECTILE_H
