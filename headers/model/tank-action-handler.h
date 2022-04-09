#ifndef TANKS_TANK_MOVE_HANDLER_H
#define TANKS_TANK_MOVE_HANDLER_H

#include <memory>
#include <queue>
#include "model/event.h"
#include "model/tank.h"

namespace Tanks::model {
class TankActionHandler {
public:
    explicit TankActionHandler(Tank &tank_,
                               std::queue<std::unique_ptr<Event>> &que_);

    void move(Direction dir);

private:
    Tank &tank;
    //    GameModel &model;
    std::queue<std::unique_ptr<Event>> &que;
    //    int lastActionTick = -1;
};
}  // namespace Tanks::model

#endif  // TANKS_TANK_MOVE_HANDLER_H
