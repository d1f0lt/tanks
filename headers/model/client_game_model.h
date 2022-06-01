#ifndef TANKS_CLIENT_GAME_MODEL_H
#define TANKS_CLIENT_GAME_MODEL_H

#include <boost/asio/ip/tcp.hpp>
#include <thread>
#include "model/game_model.h"
#include "model/thread_safe_queue.h"
#include "player_action_handler.h"

namespace Tanks::model {
using boost::asio::ip::tcp;

class ClientModel : public GameModel {
public:
    ~ClientModel() noexcept override;
    explicit ClientModel(int playerId, tcp::socket socket);

    [[nodiscard]] PlayerActionsHandler getHandler();

    [[nodiscard]] std::optional<std::reference_wrapper<Tank>> tank();
    void finishGame() override;

protected:
    void receiveEvents();
    void executeAllEvents() override;

    void addEvent(std::unique_ptr<Event> event);

private:
    const int playerId_;
    tcp::socket socket_;
    //    std::thread receiver_;

    SafeQueue<std::unique_ptr<Event>> events_;
    SafeQueue<int> tickSize_;
};
}  // namespace Tanks::model

#endif  // TANKS_CLIENT_GAME_MODEL_H
