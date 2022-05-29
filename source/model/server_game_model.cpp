#include "model/server_game_model.h"
#include <thread>
#include "model/event.h"
#include "model/network_utils.h"
#include "model/projectile.h"
#include "model/spawners.h"
#include "model/tank.h"

namespace Tanks::model {
using boost::asio::ip::tcp;

int ServerModel::addPlayer(tcp::socket &socket) {
    int id = getIncrId();
    playersSockets_.emplace(id, socket);

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
    acceptSpawners();

    std::queue<std::unique_ptr<Event>> eventsToSend;
    while (!events_.empty()) {
        auto event = std::move(events_.front());
        events_.pop();
        executeEvent(*event);
        eventsToSend.emplace(std::move(event));
    }

    for (auto id : bots_) {
        auto event = getEventByBot(id);
        executeEvent(*event);
        eventsToSend.emplace(std::move(event));
    }

    sendEventsToClients(std::move(eventsToSend));
}

std::unique_ptr<Event> ServerModel::getEventByBot(int botId) {
    return std::make_unique<TankMove>(
        botId, Direction::RIGHT,
        dynamic_cast<Tank &>(getById(botId)->get()).getSpeed());
}

void ServerModel::acceptSpawners() {
    for (const auto &spawner : spawners_) {
        spawner->action();
    }
}
}  // namespace Tanks::model
