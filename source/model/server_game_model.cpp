#include "model/server_game_model.h"
#include <thread>
#include "model/event.h"
#include "model/network_utils.h"
#include "model/projectile.h"
#include "model/spawners.h"
#include "model/tank.h"

namespace Tanks::model {
using boost::asio::ip::tcp;

int ServerModel::addPlayer(tcp::socket &socket, PlayerSkills skills) {
    int id = getDecrId();
    playersSockets_.emplace(id, socket);
    setPlayerSkills(id, skills);

    spawners_.emplace_back(std::make_unique<MediumTankSpawner>(*this, id));
    //    events_.emplace(std::make_unique<SpawnTank>(id, TILE_SIZE, TILE_SIZE,
    //                                                EntityType::MEDIUM_TANK));
    std::thread([&]() { receiveTurns(socket); }).detach();
    return id;
}

void ServerModel::receiveTurns(tcp::socket &client) {
    try {
        while (true) {
            auto event = readEvent(client);
            if (!event) {
                continue;
            }
            std::shared_lock lock(getMutex());
            events_.emplace(std::move(event));
        }
    } catch (boost::system::system_error &e) {
        std::string msg(e.what());
        if (msg == "read: Bad file descriptor [system:9]" ||
            msg == "read: Connection reset by peer [system:104]" ||
            msg == "read: End of file [asio.misc:2]" ||
            msg == "write_some: Bad file descriptor [system:9]") {
            return;  // Connection was closed
        }
        throw e;
    }
}

void ServerModel::sendEventsToClients(
    std::queue<std::unique_ptr<Event>> events) {
    for (auto &[id, socket] : playersSockets_) {
        sendInt(socket, events.size());
    }

    while (!events.empty()) {
        auto event = std::move(events.front());
        events.pop();
        for (auto &[id, socket] : playersSockets_) {
            event->sendTo(socket);
        }
    }
}

void ServerModel::executeAllEvents() {
    std::queue<std::unique_ptr<Event>> eventsToSend;

    for (const auto &spawner : spawners_) {
        if (spawner->isSpawnNow()) {
            auto event = spawner->createEvent();
            executeEvent(*event);
            eventsToSend.emplace(std::move(event));
        }
        spawner->nextTick();
    }

    while (!events_.empty()) {
        auto event = std::move(events_.front());
        events_.pop();
        executeEvent(*event);
        eventsToSend.emplace(std::move(event));
    }

    for (auto id : bots_) {
        if (getById(id)) {
            auto event = getEventByBot(id);
            executeEvent(*event);
            eventsToSend.emplace(std::move(event));
        }
    }

    sendEventsToClients(std::move(eventsToSend));
}

std::unique_ptr<Event> ServerModel::getEventByBot(int botId) {
    int rnd = getRnd() % 20;
    if (rnd >= 12) {
        return std::make_unique<TankShoot>(
            botId, dynamic_cast<Tank &>(getById(botId)->get()).getDirection());
    } else {
        auto directon = static_cast<Direction>(rnd % 4);
        return std::make_unique<TankMove>(
            botId, directon,
            dynamic_cast<Tank &>(getById(botId)->get()).getSpeed());
    }
}

PlayerSkills ServerModel::getPlayerSkills(int id) const {
    return players_.at(id);
}

void ServerModel::setPlayerSkills(int id, PlayerSkills skills) {
    players_.emplace(id, skills);
}

void ServerModel::addEvent(std::unique_ptr<Event> event) {
    events_.emplace(std::move(event));
}

ServerModel::ServerModel(int level, int botsCount) {
    loadLevel(level);
    for (int i = 0; i < botsCount; i++) {
        addBot();
    }
}

void ServerModel::addBot() {
    auto id = getDecrId();
    bots_.emplace(id);
    players_.emplace(id, PlayerSkills());
    spawners_.emplace_back(std::make_unique<MediumTankSpawner>(*this, id));
}

DecrId ServerModel::getDecrId() {
    return decrId--;
}

}  // namespace Tanks::model
