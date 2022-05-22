#ifndef TANKS_SERVER_GAME_MODEL_H
#define TANKS_SERVER_GAME_MODEL_H

#include <boost/asio/ip/tcp.hpp>
#include "model/game_model.h"
#include "model/playable-tank.h"

namespace Tanks::model {
class ServerModel : public GameModel {
public:
    int addPlayer(boost::asio::ip::tcp::socket &ios);

private:
    std::queue<std::unique_ptr<Event>> events_;

    void listen(boost::asio::ip::tcp::socket &client);
    void executeAllEvents() override;
};
}  // namespace Tanks::model

#endif  // TANKS_SERVER_GAME_MODEL_H
