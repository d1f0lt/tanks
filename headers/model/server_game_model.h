#ifndef TANKS_SERVER_GAME_MODEL_H
#define TANKS_SERVER_GAME_MODEL_H

#include <boost/asio/ip/tcp.hpp>
#include <shared_mutex>
#include <unordered_set>
#include "model/game_model.h"
#include "model/spawners.h"

namespace Tanks::model {
using boost::asio::ip::tcp;

class ServerModel : public GameModel {
    friend Spawner;

public:
    [[nodiscard]] int addPlayer(tcp::socket &socket);

private:
    std::queue<std::unique_ptr<Event>> events_;
    std::unordered_map<int, tcp::socket &> playersSockets_;
    std::unordered_set<int> bots_;
    std::vector<std::unique_ptr<Spawner>> spawners_;

    void receiveTurns(tcp::socket &client);
    void executeAllEvents() override;
    void acceptSpawners();

    [[nodiscard]] std::unique_ptr<Event> getEventByBot(int botId);

    void sendEventsToClients(std::queue<std::unique_ptr<Event>> events);
};
}  // namespace Tanks::model

#endif  // TANKS_SERVER_GAME_MODEL_H
