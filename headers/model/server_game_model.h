#ifndef TANKS_SERVER_GAME_MODEL_H
#define TANKS_SERVER_GAME_MODEL_H

#include <boost/asio/ip/tcp.hpp>
#include <shared_mutex>
#include <unordered_set>
#include "model/game_model.h"

namespace Tanks::model {
using boost::asio::ip::tcp;

class ServerModel : public GameModel {
public:
    [[nodiscard]] int addPlayer(boost::asio::ip::tcp::socket &socket);

private:
    std::queue<std::unique_ptr<Event>> events_;
    std::unordered_map<int, tcp::socket &> players_;
    std::unordered_set<int> bots_;
    std::shared_mutex sharedMuxtex;

    void receiveTurns(tcp::socket &client);
    void executeAllEvents() override;

    [[nodiscard]] std::unique_ptr<Event> getEventByBot(int botId);

    void sendEventsToClients(std::queue<std::unique_ptr<Event>> events);
};
}  // namespace Tanks::model

#endif  // TANKS_SERVER_GAME_MODEL_H
