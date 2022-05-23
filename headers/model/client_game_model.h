#ifndef TANKS_CLIENT_GAME_MODEL_H
#define TANKS_CLIENT_GAME_MODEL_H

#include <boost/asio/ip/tcp.hpp>
#include "model/game_model.h"

namespace Tanks::model {
using boost::asio::ip::tcp;

class ClientModel : public GameModel {
public:
    explicit ClientModel(int playerId, tcp::socket socket);

    [[nodiscard]] PlayerActionsHandler getHandler();

private:
    const int playerId_;
    tcp::socket socket_;
};
}  // namespace Tanks::model

#endif  // TANKS_CLIENT_GAME_MODEL_H
