#ifndef TANKS_PLAYABLE_TANK_H
#define TANKS_PLAYABLE_TANK_H

#include <memory>
#include <queue>
#include "model/event.h"
#include "model/tank.h"

namespace Tanks::model {
class PlayableTank : public Tank {
public:
    PlayableTank(int left,
                 int top,
                 Direction dir,
                 GameMap &map_,
                 std::queue<std::unique_ptr<Event>> &que_);
    // TODO: ShootHandler or other solution for shooting

    void move(Direction dir);

private:
    std::queue<std::unique_ptr<Event>> &que;
    // TODO MoveHandler
};
}  // namespace Tanks::model
#endif  // TANKS_PLAYABLE_TANK_H
