#ifndef TANKS_SERVER_GAME_MODEL_H
#define TANKS_SERVER_GAME_MODEL_H

#include <boost/asio/ip/tcp.hpp>
#include <thread>
#include <unordered_set>
#include "model/game_model.h"
#include "model/spawners.h"
#include "model/thread_safe_queue.h"
#include "player_skills.h"

namespace Tanks::model {
using boost::asio::ip::tcp;
using Menu::PlayerSkills;

class ServerModel : public GameModel {
    friend Spawner;

public:
    ~ServerModel() override;

    void finishGame() noexcept final;

    explicit ServerModel(int level = 1, int botsCount = 0, int bonuses = 0);
    explicit ServerModel(const std::string &level,
                         int botsCount = 0,
                         int bonuses = 0);

    [[nodiscard]] int addPlayer(tcp::socket &socket, PlayerSkills skills = {});

    [[nodiscard]] PlayerSkills getPlayerSkills(int id) const;

protected:
    void addEvent(std::unique_ptr<Event> event);
    struct Player {
        tcp::socket &socket_;
        std::thread receiver_;
    };

private:
    SafeQueue<std::unique_ptr<Event>> events_;
    std::unordered_map<int, Player> players_;
    std::unordered_set<int> bots_;
    std::vector<std::unique_ptr<Spawner>> spawners_;
    std::unordered_map<int, PlayerSkills> tanksSkills_;
    DecrId decrId{-1};

    void receiveTurns(tcp::socket &client);
    void executeAllEvents() override;

    [[nodiscard]] std::unique_ptr<Event> getEventByBot(int botId);

    void sendEventsToClients(const std::vector<std::unique_ptr<Event>> &events);

    [[nodiscard]] DecrId getDecrId();

    void setPlayerSkills(int id, PlayerSkills skills);
    void addBot();
};
}  // namespace Tanks::model

#endif  // TANKS_SERVER_GAME_MODEL_H
