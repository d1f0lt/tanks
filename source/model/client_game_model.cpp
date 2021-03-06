#include "model/client_game_model.h"
#include <thread>
#include "model/network_utils.h"
#include "model/tank.h"

#ifndef NDEBUG
#include <fstream>
#include <iostream>
#endif

namespace Tanks::model {

ClientModel::ClientModel(int playerId, int lives, tcp::socket socket)
    : playerId_(playerId),
      playerLives_(lives),
      socket_(std::move(socket)),
      eventReceiver_([&]() { receiveEvents(); }){};

PlayerActionsHandler ClientModel::getHandler() {
    return PlayerActionsHandler(playerId_, *this, socket_);
}

// TODO: no copypast
void ClientModel::addEvent(std::unique_ptr<Event> event) {
    events_.Produce(std::move(event));
}

void ClientModel::receiveEvents() {
    auto &tickSize = tickSize_;
    auto &socket = socket_;
    try {
        while (true) {
            int count = receiveInt(socket);
            tickSize.Produce(count);
            for (int i = 0; i < count; i++) {
                addEvent(readEvent(socket));
            }
        }
    } catch (boost::system::system_error &) {
        return;
    } catch (std::system_error &e) {
#ifndef NDEBUG
        static std::fstream log("log_client.txt", std::ios_base::app);
        log << getTick() << ' ' << e.what() << std::endl;
#endif
        return;
    }
}

void ClientModel::executeAllEvents() {
#ifdef MODEL_LOGS
    std::ofstream file("clientEvents.txt", std::ios_base::app);
#endif
    std::unique_ptr<Event> event;
    int cnt = -1;
    int ticks = std::max(1, static_cast<int>(tickSize_.Size()));
    for (int i = 0; i < ticks && !getIsFinished(); i++) {
        if (!tickSize_.ConsumeSync(cnt)) {
            return;
        }
        for (; cnt > 0; cnt--) {
            bool res = events_.ConsumeSync(event);
            assert(res);
#ifdef MODEL_LOGS
            file << getTick() << ' ';
            printEvent(*event, file);
            file << std::endl;
#endif
            executeEvent(*event);
        }
        moveBullets();
        incrTick();
    }
}

ClientModel::~ClientModel() noexcept {
    ClientModel::finishGame();
}

std::optional<std::reference_wrapper<Tank>> ClientModel::tank() {
    auto entity = getById(playerId_);
    if (!entity) {
        return std::nullopt;
    }
    return dynamic_cast<Tank &>(entity->get());
}

void ClientModel::finishGame() noexcept {
    if (getIsFinished()) {
        return;
    }
    setFinished();
    std::unique_lock lock(getMutex());
    safeShutdown(socket_);
    eventReceiver_.join();
}

void ClientModel::nextTick() {
    if (getIsFinished()) {
        return;
    }
    std::unique_lock lock(getMutex());
    setWasDestroyedBlockThisTurn(false);
    setWasShootThisTurn(false);
    setWasTankDestroyed(false);
    executeAllEvents();
    lock.unlock();
    getCondvar().notify_all();
}

void ClientModel::eraseEntity(Entity &entity) {
    if (entity.getId() == playerId_ && playerLives_ != INFINITE_LIVES) {
        playerLives_--;
    }
    GameModel::eraseEntity(entity);
}

int ClientModel::getLives() const {
    return playerLives_;
}

int ClientModel::getKills() const {
    return kills_;
}
// namespace Tanks::model
}  // namespace Tanks::model