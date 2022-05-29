#ifndef TANKS_SERVER_GAME_MODEL_H
#define TANKS_SERVER_GAME_MODEL_H

#include <boost/asio/ip/tcp.hpp>
#include <shared_mutex>
#include <unordered_set>
#include "model/game_model.h"
#include "model/spawners.h"

namespace Tanks::model {
using boost::asio::ip::tcp;

struct PlayerSkillsTmp {
    const int tankSpeed = DEFAULT_TANK_SPEED;
    const int bulletSpeed = DEFAULT_BULLET_SPEED;
    const int reloadTicks = DEFAULT_RELOAD_TICKS;
};

using PlayerSkills = PlayerSkillsTmp;

class ServerModel : public GameModel {
    friend Spawner;

public:
    [[nodiscard]] int addPlayer(tcp::socket &socket, PlayerSkills skills = {});

private:
    std::queue<std::unique_ptr<Event>> events_;
    std::unordered_map<int, tcp::socket &> playersSockets_;
    std::unordered_set<int> bots_;
    std::vector<std::unique_ptr<Spawner>> spawners_;
    std::unordered_map<int, PlayerSkills> players_;

    void receiveTurns(tcp::socket &client);
    void executeAllEvents() override;
    void acceptSpawners();

    [[nodiscard]] std::unique_ptr<Event> getEventByBot(int botId);

    void sendEventsToClients(std::queue<std::unique_ptr<Event>> events);

    [[nodiscard]] PlayerSkills getPlayerSkills(int id) const;
    void setPlayerSkills(int id, PlayerSkills skills);
};
}  // namespace Tanks::model

#endif  // TANKS_SERVER_GAME_MODEL_H
