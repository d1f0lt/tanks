#include "model/server_game_model.h"
#include <thread>
#include "model/projectile.h"

namespace Tanks::model {
PlayableTank &ServerModel::spawnPlayableTank(int left,
                                             int top,
                                             boost::asio::ip::tcp::socket &os) {
    return spawnPlayableTank(left, top, getCurrentId(), os);
}

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
    int id = getCurrentId();
    auto &tank = spawnPlayableTank(TILE_SIZE, TILE_SIZE, id, ios);
    std::thread([&]() { listen(ios); }).detach();
    return id;
}

PlayableTank &ServerModel::spawnPlayableTank(int left,
                                             int top,
                                             int id,
                                             boost::asio::ip::tcp::socket &os) {
    assert(left + TANK_SIZE < map_.getWidth());
    assert(top + TANK_SIZE < map_.getHeight());

    for (int row = top; row < top + TANK_SIZE; row++) {
        for (int col = left; col < left + TANK_SIZE; col++) {
            assert(getByCoords(col, row).isTankPassable());
        }
    }

    addEntity(std::make_unique<PlayableTank>(left, top, id, Direction::UP, os,
                                             *this));

    return dynamic_cast<PlayableTank &>(getByCoords(left, top));
}

void ServerModel::executeEvents() {
    while (!events_.empty()) {
        auto event = std::move(events_.front());
        events_.pop();
        if (auto *ptr = dynamic_cast<TankMove *>(event.get())) {
            auto ent = getById(ptr->getId());
            if (!ent) {
                continue;
            }

            auto &handler = dynamic_cast<TankHandler &>(getHandler(*ent));
            handler.move(ptr->getDirection(), ptr->getSpeed());
        }
    }

    for (auto *entity :
         getAllByLink()[static_cast<unsigned>(EntityType::BOT_TANK)]) {
        auto *tank = dynamic_cast<BotTank *>(entity);
        assert(tank != nullptr);
        dynamic_cast<MovableHandler &>(getHandler(*tank))
            .move(tank->getDirection(), tank->getSpeed());
    }

    for (auto *entity :
         getAllByLink()[static_cast<unsigned>(EntityType::BULLET)]) {
        auto *bullet = dynamic_cast<Projectile *>(entity);
        assert(bullet != nullptr);

        if (dynamic_cast<ProjectileHandler &>(getHandler(*bullet))
                .isBreakOnNextTick()) {
            continue;
        }
        dynamic_cast<MovableHandler &>(getHandler(*bullet))
            .move(bullet->getDirection(), bullet->getSpeed());
    }
}
}  // namespace Tanks::model
