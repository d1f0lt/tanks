#include "model/player_action_handler.h"
#include "model/client_game_model.h"
#include "model/event.h"
#include "model/tank.h"

namespace Tanks::model {
PlayerActionsHandler::PlayerActionsHandler(int playerId,
                                           ClientModel &model,
                                           tcp::socket &socket)
    : id_(playerId), model_(model), socket_(socket) {
}

void PlayerActionsHandler::move(Direction direction, int speed) {
    if (!model_.tank()) {
        return;
    }
    TankMove(id_, direction, speed).sendTo(socket_);
}

void PlayerActionsHandler::setDirection(Direction direction) {
    if (!model_.tank()) {
    }
    move(direction, 0);
}

void PlayerActionsHandler::shoot(Direction direction) {
    if (!model_.tank()) {
        return;
    }
    TankShoot(id_, direction).sendTo(socket_);
}

void PlayerActionsHandler::move(Direction direction) {
    if (!model_.tank()) {
        return;
    }
    TankMove(id_, direction, model_.tank()->get().getSpeed()).sendTo(socket_);
}

void PlayerActionsHandler::shoot() {
    if (!model_.tank()) {
        return;
    }
    TankShoot(id_, model_.tank()->get().getDirection()).sendTo(socket_);
}

std::optional<std::reference_wrapper<Tank>> PlayerActionsHandler::tank() {
    return model_.tank();
}

const GameModel &PlayerActionsHandler::getModel() const {
    return model_;
}

int PlayerActionsHandler::getLives() const {
    return model_.getLives();
}
}  // namespace Tanks::model