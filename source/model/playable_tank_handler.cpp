#include "model/playable_tank_handler.h"
#include "model/event.h"
#include "model/playable-tank.h"

namespace Tanks::model {
PlayableTankHandler::PlayableTankHandler(GameModel &model,
                                         PlayableTank &tank,
                                         boost::asio::ip::tcp::socket &os)
    : TankHandler(model, tank), os_(os) {
}

void PlayableTankHandler::sendMove(Direction direction, int speed) {
    TankMove::sendTo(os_, getEntity().getId(), direction, speed);
}
}  // namespace Tanks::model