#include "model/tank-action-handler.h"
#include "model/game_model.h"

namespace Tanks::model {
TankActionHandler::TankActionHandler(Tank &tank_, GameModel &model_)
    : tank(tank_), model(model_) {
}

void TankActionHandler::move(Tanks::model::Direction dir) {
    model.moveEntity(tank, dir);
    // TODO 1 action per tick
}
}  // namespace Tanks::model