#ifndef TANKS_PLAYABLE_TANK_HANDLER_H
#define TANKS_PLAYABLE_TANK_HANDLER_H

#include <boost/asio/ip/tcp.hpp>
#include "model/handler.h"

namespace Tanks::model {

class PlayableTankHandler : public TankHandler {
public:
    explicit PlayableTankHandler(GameModel &model,
                                 PlayableTank &tank,
                                 boost::asio::ip::tcp::socket &os);

    void sendMove(Direction direction, int speed);

private:
    boost::asio::ip::tcp::socket &os_;
};
}  // namespace Tanks::model

#endif  // TANKS_PLAYABLE_TANK_HANDLER_H
