#include "model/client_game_model.h"
namespace Tanks::model {
ClientModel::ClientModel(int playerId, tcp::socket socket)
    : playerId_(playerId), socket_(std::move(socket)) {
}

PlayerActionsHandler ClientModel::getHandler() {
    return PlayerActionsHandler(playerId_, socket_);
}
}  // namespace Tanks::model
