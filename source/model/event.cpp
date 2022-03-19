#include "model/event.h"
namespace Tanks::model {
void Tanks::model::TankMove::execute(Tanks::model::GameModel &model) {
    Tank.move(direction);
}
}  // namespace Tanks::model
