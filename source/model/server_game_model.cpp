#include "model/server_game_model.h"
#include <thread>
#include "model/projectile.h"

namespace Tanks::model {
using boost::asio::ip::tcp;

int ServerModel::addPlayer(tcp::socket &socket) {
    int id = getIncrId();
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
            events_.emplace(std::move(event));
        }
    } catch (boost::system::system_error &e) {
        if (std::string(e.what()) ==
            std::string("read: Bad file descriptor [system:9]")) {
            return;  // Connection was closed
        }
        throw e;
    }
}

void ServerModel::executeAllEvents() {
    while (!events_.empty()) {
        auto event = std::move(events_.front());
        events_.pop();
        executeEvent(*event);
    }

    for (auto *entity :
         getAllByLink()[static_cast<unsigned>(EntityType::BOT_TANK)]) {
        auto *tank = dynamic_cast<Tank *>(entity);
        assert(tank != nullptr);
        dynamic_cast<MovableHandler &>(getHandler(*tank))
            .move(tank->getDirection(), tank->getSpeed());
    }
}
}  // namespace Tanks::model
