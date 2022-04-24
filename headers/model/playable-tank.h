#ifndef TANKS_PLAYABLE_TANK_H
#define TANKS_PLAYABLE_TANK_H

#include <memory>
#include <queue>
#include "model/tank.h"

namespace Tanks::model {
class PlayableTank : public Tank {
public:
    PlayableTank(int left,
                 int top,
                 Direction dir,
                 std::unique_ptr<BasicHandler> handler_);

    using MovableEntity::move;
    using MovableEntity::setDirection;
    using Tank::shoot;

private:
};
}  // namespace Tanks::model
#endif  // TANKS_PLAYABLE_TANK_H
