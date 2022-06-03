#ifndef TANKS_PLAYER_ACTION_HANDLER_H
#define TANKS_PLAYER_ACTION_HANDLER_H

#include <boost/asio/ip/tcp.hpp>
#include "model/handler.h"
#include "model/player_action_handler_fwd.h"

namespace Tanks::model {
class ClientModel;
using boost::asio::ip::tcp;

class PlayerActionsHandler {
public:
    explicit PlayerActionsHandler(int id,
                                  ClientModel &model,
                                  tcp::socket &socket);

    void move(Direction direction, int speed);
    void move(Direction direction);
    void setDirection(Direction direction);

    void shoot(Direction direction);
    void shoot();

    [[nodiscard]] std::optional<std::reference_wrapper<Tank>> tank();

private:
    const int id_;
    ClientModel &model_;
    tcp::socket &socket_;
};

}  // namespace Tanks::model

#endif  // TANKS_PLAYER_ACTION_HANDLER_H
