#include "model/server_game_model.h"
#include <thread>
#include "model/event.h"
#include "model/projectile.h"

namespace Tanks::model {
using boost::asio::ip::tcp;

int ServerModel::addPlayer(tcp::socket &socket) {
    int id = getIncrId();
    // TODO sendInt
    players_.emplace(id, socket);
    events_.emplace(
        std::make_unique<SpawnTank>(id, TILE_SIZE, TILE_SIZE, Direction::LEFT));
    //    auto &tank = spawnPlayableTank(TILE_SIZE, TILE_SIZE, id, socket);
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
            msg == "read: Connection reset by peer [system:104]") {
            return;  // Connection was closed
        }
        throw e;
    }
}

namespace {
template <typename T>
void sendInt(boost::asio::ip::tcp::socket &os, const T &a) {
    static thread_local auto tmp = static_cast<std::int32_t>(a);
    os.write_some(
        boost::asio::buffer(reinterpret_cast<const char *>(&tmp), sizeof(T)));
}
}  // namespace

void ServerModel::sendEventsToClients(
    std::queue<std::unique_ptr<Event>> events) {
    for (auto &[id, socket] : players_) {
        sendInt(socket, events.size());
    }

    while (!events.empty()) {
        auto event = std::move(events.front());
        events.pop();
        for (auto &[id, socket] : players_) {
            event->sendTo(socket);
        }
    }
}

void ServerModel::executeAllEvents() {
    std::queue<std::unique_ptr<Event>> eventsToSend;
    while (!events_.empty()) {
        auto event = std::move(events_.front());
        events_.pop();
        executeEvent(*event);
        eventsToSend.emplace(std::move(event));
    }

    for (auto id : bots_) {
        auto event = eventByBot(id);
        executeEvent(*event);
        eventsToSend.emplace(std::move(event));
    }
    sendEventsToClients(std::move(eventsToSend));
}

std::unique_ptr<Event> ServerModel::eventByBot(int botId) {
    return std::make_unique<TankMove>(
        botId, Direction::RIGHT,
        dynamic_cast<Tank &>(getById(botId)->get()).getSpeed());
}
}  // namespace Tanks::model
