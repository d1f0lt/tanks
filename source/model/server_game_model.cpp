#include "model/server_game_model.h"
#include <thread>
#include "model/event.h"
#include "model/network_utils.h"
#include "model/projectile.h"
#include "model/spawners.h"
#include "model/tank.h"

#ifndef NDEBUG
#include <fstream>
#include <iostream>
#endif

namespace Tanks::model {
using boost::asio::ip::tcp;

int ServerModel::addPlayer(tcp::socket &socket,
                           PlayerSkills skills,
                           int lives) {
    int playerId = getDecrId();
    //    playersSockets_.emplace(playerId, *socket);
    setTankSkills(playerId, skills);
    spawners_.emplace_back(
        std::make_unique<MediumTankSpawner>(*this, playerId));
    players_.emplace(
        playerId, Player{socket, std::thread([&]() { receiveTurns(socket); })});
    tankLives_.emplace(playerId, lives);
    return playerId;
}

void ServerModel::nextTick() {
    if (getIsFinished()) {
        return;
    }
    std::unique_lock lock(getMutex());
    setWasShootThisTurn(false);
    setWasDestroyedBlockThisTurn(false);
    executeAllEvents();
    moveBullets();
    incrTick();
    lock.unlock();
    getCondvar().notify_all();
}

void ServerModel::receiveTurns(tcp::socket &socket) {
    try {
        while (!getIsFinished()) {
            auto event = readEvent(socket);
            if (getIsFinished()) {
                return;
            }
            if (!event) {
                continue;
            }
            addEvent(std::move(event));
        }
    } catch (boost::system::system_error &e) {
        return;
    }
}

void ServerModel::sendEventsToClients(
    const std::vector<std::unique_ptr<Event>> &events) {
#ifdef MODEL_LOGS
    std::ofstream file("serverEvents.txt", std::ios_base::app);
    for (const auto &event : events) {
        file << getTick() << ' ';
        printEvent(*event, file);
        file << std::endl;
    }
#endif
    static auto sendAllTo =
        [](tcp::socket &socket,
           const std::vector<std::unique_ptr<Event>> &events) {
            try {
                sendInt(socket, events.size());
            } catch (boost::system::system_error &e) {
            }
            try {
                for (const auto &event : events) {
                    event->sendTo(socket);
                }
            } catch (boost::system::system_error &e) {
            }
        };

    for (auto &[id, player] : players_) {
        sendAllTo(player.socket_, events);
    }

    //    std::unordered_set<int> disconnectedUsers;
    //    for (auto &[id, socket] : playersSockets_) {
    //        try {
    //            sendInt(socket, events.size());
    //        } catch (boost::system::system_error &e) {
    //            disconnectedUsers.emplace(id);
    //        }
    //    }
    //
    //    for (auto &event : events) {
    //        for (auto &[id, socket] : playersSockets_) {
    //            event->sendTo(socket);
    //        }
    //    }
}

void ServerModel::executeAllEvents() {
    std::vector<std::unique_ptr<Event>> eventsToSend;
    eventsToSend.reserve(bots_.size() + tanksSkills_.size());

    for (const auto &spawner : spawners_) {
        if (spawner->isSpawnNow()) {
            auto event = spawner->createEvent();
            assert(event != nullptr);
            executeEvent(*event);
            eventsToSend.emplace_back(std::move(event));
            //            if (executeEvent(*event)) {
            //                eventsToSend.emplace_back(std::move(event));
            //            }
        }
        spawner->nextTick();
    }

    int size = static_cast<int>(events_.Size());

    for (; size > 0; --size) {
        std::unique_ptr<Event> event;
        if (!events_.Consume(event)) {
            assert(false);
            throw 123;
        }
        if (executeEvent(*event)) {
            eventsToSend.emplace_back(std::move(event));
        }
    }

    for (auto botId : bots_) {
        if (getById(botId)) {
            auto event = getEventByBot(botId);
            if (executeEvent(*event)) {
                eventsToSend.emplace_back(std::move(event));
            }
        }
    }

    sendEventsToClients(eventsToSend);
}

#ifdef EPILEPTIC_BOTS
std::unique_ptr<Event> ServerModel::getEventByBot(int botId) {
    int rnd = getRnd() % 20;
    if (rnd >= 12) {
        return std::make_unique<TankShoot>(
            botId, dynamic_cast<Tank &>(getById(botId)->get()).getDirection());
    } else {
        auto direction = static_cast<Direction>(rnd % 4);
        return std::make_unique<TankMove>(
            botId, direction,
            dynamic_cast<Tank &>(getById(botId)->get()).getSpeed());
    }
}
#else

std::unique_ptr<Event> ServerModel::getEventByBot(int botId) {
    int rnd = getRnd() % 80;
    if (rnd >= 70) {
        return std::make_unique<TankShoot>(
            botId, dynamic_cast<Tank &>(getById(botId)->get()).getDirection());
    } else if (rnd <= 3) {
        auto direction = static_cast<Direction>(rnd % 4);
        return std::make_unique<TankMove>(botId, direction, 0);
    } else {
        return std::make_unique<TankMove>(
            botId, dynamic_cast<Tank &>(getById(botId)->get()).getDirection(),
            dynamic_cast<Tank &>(getById(botId)->get()).getSpeed());
    }
}

#endif

PlayerSkills ServerModel::getTankSkills(int tankId) const {
    return tanksSkills_.at(tankId);
}

void ServerModel::setTankSkills(int tankId, PlayerSkills skills) {
    tanksSkills_.emplace(tankId, skills);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
ServerModel::ServerModel(int level, int botsCount, int bonuses) {
    loadLevel(level);
    for (int i = 0; i < botsCount; i++) {
        addBot();
    }
    for (int i = 0; i < bonuses; i++) {
        spawners_.emplace_back(std::make_unique<BonusSpawner>(
            *this, getDecrId(), EntityType::WALK_ON_WATER_BONUS));
    }
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
ServerModel::ServerModel(const std::string &level, int botsCount, int bonuses) {
    loadLevel(level);
    for (int i = 0; i < botsCount; i++) {
        addBot();
    }
    for (int i = 0; i < bonuses; i++) {
        spawners_.emplace_back(std::make_unique<BonusSpawner>(
            *this, getDecrId(), EntityType::WALK_ON_WATER_BONUS));
    }
}

void ServerModel::addBot() {
    auto botId = getDecrId();
    bots_.emplace(botId);
    PlayerSkills skills;
    tanksSkills_.emplace(botId, skills);
    spawners_.emplace_back(std::make_unique<MediumTankSpawner>(*this, botId));
    tankLives_.emplace(botId, INFINITE_LIVES);
}

DecrId ServerModel::getDecrId() {
    return decrId--;
}

void ServerModel::addEvent(std::unique_ptr<Event> event) {
    events_.Produce(std::move(event));
}

void ServerModel::finishGame() noexcept {
    if (getIsFinished()) {
        return;
    }
    setFinished();
    std::unique_lock<std::mutex> modelLock(getMutex(), std::defer_lock);
    getCondvar().wait(modelLock, [&]() { return modelLock.try_lock(); });
    assert(modelLock);
    std::vector<std::unique_ptr<Event>> vec;
    vec.emplace_back(std::make_unique<GameEnd>(1));
    sendEventsToClients(vec);
    for (auto &[id, player] : players_) {
        safeShutdown(player.socket_);
        player.receiver_.join();
    }
}

ServerModel::~ServerModel() {
    finishGame();
}

int ServerModel::getLives(int tankId) const {
    assert(tankLives_.count(tankId) == 1);
    return tankLives_.at(tankId);
}

void ServerModel::decrLives(int tankId) {
    int &lives = tankLives_[tankId];
    if (lives == INFINITE_LIVES || lives == 0) {
        return;
    }
    lives--;
}

bool ServerModel::executeEvent(Event &event) {
    return event.acceptExecutor(ServerEventExecutor(*this));
}

}  // namespace Tanks::model
