#include "model/client_game_model.h"
#include "model/network_utils.h"

namespace Tanks::model {
ClientModel::ClientModel(int playerId, tcp::socket socket)
    : playerId_(playerId), socket_(std::move(socket)) {
}

PlayerActionsHandler ClientModel::getHandler() {
    return PlayerActionsHandler(playerId_, socket_);
}

void ClientModel::executeAllEvents() {
    try {
        auto count = receiveInt(socket_);
        for (int i = 0; i < count; i++) {
            auto event = readEvent(socket_);
            executeEvent(*event);
        }
    } catch (boost::system::system_error &e) {
        if (std::string(e.what()) == "read: End of file [asio.misc:2]") {
            return;
        }
        throw e;
    }
}
}  // namespace Tanks::model
