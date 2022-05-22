#ifndef TANKS_SERVER_GAME_MODEL_H
#define TANKS_SERVER_GAME_MODEL_H

#include <boost/asio/ip/tcp.hpp>
#include "model/game_model.h"
#include "model/playable-tank.h"

namespace Tanks::model {
class ServerModel : public GameModel {
    [[nodiscard]] PlayableTank &spawnPlayableTank(
        int left,
        int top,
        int id,
        boost::asio::ip::tcp::socket &os);
    void listen(boost::asio::ip::tcp::socket &client);

protected:
    void executeEvents() override;

private:
    std::queue<std::unique_ptr<Event>> events_;

public:
    [[nodiscard]] PlayableTank &
    spawnPlayableTank(int left, int top, boost::asio::ip::tcp::socket &os);

    int addPlayer(boost::asio::ip::tcp::socket &ios);
};
}  // namespace Tanks::model

#endif  // TANKS_SERVER_GAME_MODEL_H
