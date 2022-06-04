#ifndef TANKS_CLIENT_GAME_MODEL_H
#define TANKS_CLIENT_GAME_MODEL_H

#include <boost/asio/ip/tcp.hpp>
#include <thread>
#include "model/game_model.h"
#include "model/player_action_handler.h"
#include "model/thread_safe_queue.h"
#include "player_skills.h"

namespace Tanks::model {
using boost::asio::ip::tcp;
using Menu::PlayerSkills;

class ClientModel : public GameModel {
public:
    explicit ClientModel(int playerId, tcp::socket socket);
    ~ClientModel() noexcept override;

    [[nodiscard]] PlayerActionsHandler getHandler();

    [[nodiscard]] std::optional<std::reference_wrapper<Tank>> tank();
    void finishGame() noexcept override;

    void nextTick() override;

protected:
    void receiveEvents();
    void executeAllEvents() override;

    void addEvent(std::unique_ptr<Event> event);

private:
    const int playerId_;
    tcp::socket socket_;

    SafeQueue<std::unique_ptr<Event>> events_;
    SafeQueue<int> tickSize_;
    std::thread receiver_;
};
}  // namespace Tanks::model

#endif  // TANKS_CLIENT_GAME_MODEL_H
