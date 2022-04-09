#include "model/tank-action-handler.h"
#include "model/tank-move-event.h"

namespace Tanks::model {
TankActionHandler::TankActionHandler(Tank &tank_,
                                     std::queue<std::unique_ptr<Event>> &que_)
    : tank(tank_), que(que_) {
}

void TankActionHandler::move(Tanks::model::Direction dir) {
    // TODO 1 action per tick
    que.push(std::make_unique<TankMove>(tank, dir));
}
}  // namespace Tanks::model