#include "model/playable_tank_handler.h"
#include "model/event.h"

namespace Tanks::model {
PlayerActionsHandler::PlayerActionsHandler(int id, tcp::socket &socket)
    : id_(id), socket_(socket) {
}

void PlayerActionsHandler::move(Direction direction, int speed) {
    TankMove::sendTo(socket_, id_, direction, speed);
}

void PlayerActionsHandler::changeDirection(Direction direction) {
    move(direction, 0);
}

}  // namespace Tanks::model