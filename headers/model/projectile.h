#ifndef TANKS_PROJECTILE_H
#define TANKS_PROJECTILE_H

#include "movable_entity.h"

namespace Tanks::model {
class Projectile : public MovableEntity {
    friend GameModel;

public:
    explicit Projectile(int col,
                        int row,
                        Direction dir,
                        std::unique_ptr<BasicHandler> handler_);

private:
};
}  // namespace Tanks::model

#endif  // TANKS_PROJECTILE_H
