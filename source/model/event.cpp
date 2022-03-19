#include "model/event.h"
#include "model/movable_entity.h"
#include "model/tank.h"

namespace Tanks::model {
void Tanks::model::TankMove::execute(Tanks::model::GameModel &model) {
    ((MovableEntity &)tank).move(direction);
}

TankMove::TankMove(Tank &tank_, Direction direction_)
    : tank(tank_), direction(direction_) {
}
}  // namespace Tanks::model
