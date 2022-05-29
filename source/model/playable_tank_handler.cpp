#include "model/event.h"
#include "model/player_action_handler.h"

namespace Tanks::model {
PlayerActionsHandler::PlayerActionsHandler(int id, tcp::socket &socket)
    : id_(id), socket_(socket) {
}

void PlayerActionsHandler::move(Direction direction, int speed) {
    TankMove(id_, direction, speed).sendTo(socket_);
}

void PlayerActionsHandler::changeDirection(Direction direction) {
    move(direction, 0);
}

void PlayerActionsHandler::shoot(Direction direction) {
    TankShoot(id_, direction).sendTo(socket_);
}

}  // namespace Tanks::model