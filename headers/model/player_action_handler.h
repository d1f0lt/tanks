#ifndef TANKS_PLAYER_ACTION_HANDLER_H
#define TANKS_PLAYER_ACTION_HANDLER_H

#include <boost/asio/ip/tcp.hpp>
#include "model/handler.h"
#include "model/player_action_handler_fwd.h"

namespace Tanks::model {
using boost::asio::ip::tcp;

class PlayerActionsHandler {
public:
    explicit PlayerActionsHandler(int id, tcp::socket &socket);

    void move(Direction direction, int speed);
    void changeDirection(Direction direction);
    void shoot(Direction direction);

private:
    const int id_;
    tcp::socket &socket_;
};

}  // namespace Tanks::model

#endif  // TANKS_PLAYER_ACTION_HANDLER_H
