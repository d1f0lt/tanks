#include "model/server_game_model.h"
#include <thread>
#include "model/projectile.h"

namespace Tanks::model {
void ServerModel::listen(boost::asio::ip::tcp::socket &client) {
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

int ServerModel::addPlayer(boost::asio::ip::tcp::socket &ios) {
    int id = getIncrId();
//    events_.emplace(SpawnTank(id));
    //    auto &tank = spawnPlayableTank(TILE_SIZE, TILE_SIZE, id, ios);
    std::thread([&]() { listen(ios); }).detach();
    return id;
}

void ServerModel::executeAllEvents() {
    while (!events_.empty()) {
        auto event = std::move(events_.front());
        events_.pop();
        executeEvent(*event);
    }

    for (auto *entity :
         getAllByLink()[static_cast<unsigned>(EntityType::BOT_TANK)]) {
        auto *tank = dynamic_cast<BotTank *>(entity);
        assert(tank != nullptr);
        dynamic_cast<MovableHandler &>(getHandler(*tank))
            .move(tank->getDirection(), tank->getSpeed());
    }
}
}  // namespace Tanks::model
