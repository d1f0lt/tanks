#ifndef TANKS_PLAYABLE_TANK_H
#define TANKS_PLAYABLE_TANK_H

#include <memory>
#include <queue>
#include "model/tank-action-handler.h"
#include "model/tank.h"

namespace Tanks::model {
class PlayableTank : public Tank {
public:
    PlayableTank(int left,
                 int top,
                 Direction dir,
                 GameMap &map_,
                 GameModel &model_);
    // TODO: ShootHandler or other solution for shooting

    // hiding is feature, if upcast to MovableObject move will move
    void move(Direction dir);

private:
    TankActionHandler actionHandler;
};
}  // namespace Tanks::model
#endif  // TANKS_PLAYABLE_TANK_H
