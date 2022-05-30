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

int ServerModel::addPlayer(tcp::socket &socket, PlayerSkills skills) {
    int id = getDecrId();
    playersSockets_.emplace(id, socket);
    setPlayerSkills(id, skills);
    spawners_.emplace_back(std::make_unique<MediumTankSpawner>(*this, id));
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
            addEvent(std::move(event));
        }
    } catch (boost::system::system_error &e) {
        return;
    }
}

void ServerModel::sendEventsToClients(
    std::vector<std::unique_ptr<Event>> &events) {
#ifndef NDEBUG
    static std::fstream log("log_server.txt");
    log << events.size() << std::endl;
    //    std::cout << "s " << getTick() << ' ' << events.size() << std::endl;

#endif
    static auto sendAllTo =
        [](tcp::socket &socket,
           const std::vector<std::unique_ptr<Event>> &events) {
            try {
                sendInt(socket, events.size());
            } catch (boost::system::system_error &e) {
                auto res = e.what();
            }
            try {
                for (const auto &event : events) {
                    event->sendTo(socket);
                }
            } catch (boost::system::system_error &e) {
                auto res = e.what();
            }
        };

    for (auto &[id, socket] : playersSockets_) {
        sendAllTo(socket, events);
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
    eventsToSend.reserve(bots_.size() + players_.size());

    for (const auto &spawner : spawners_) {
        if (spawner->isSpawnNow()) {
            auto event = spawner->createEvent();
            assert(event != nullptr);
            executeEvent(*event);
            eventsToSend.emplace_back(std::move(event));
        }
        spawner->nextTick();
    }

    int size = events_.Size();

    for (; size > 0; --size) {
        std::unique_ptr<Event> event;
        if (!events_.Consume(event)) {
            assert(false);
            throw 123;
        }
        executeEvent(*event);
        eventsToSend.emplace_back(std::move(event));
    }

    for (auto id : bots_) {
        if (getById(id)) {
            auto event = getEventByBot(id);
            executeEvent(*event);
            eventsToSend.emplace_back(std::move(event));
        }
    }

    sendEventsToClients(eventsToSend);
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

void ServerModel::addBot() {
    auto id = getDecrId();
    bots_.emplace(id);
    players_.emplace(id, PlayerSkills());
    spawners_.emplace_back(std::make_unique<MediumTankSpawner>(*this, id));
}

DecrId ServerModel::getDecrId() {
    return decrId--;
}

void ServerModel::addEvent(std::unique_ptr<Event> event) {
    events_.Produce(std::move(event));
}

ServerModel::~ServerModel() {
    //    finishGame();
}

void ServerModel::finishGame() {
    for (auto &[id, socket] : playersSockets_) {
        sendInt(socket, -1);
    }
    playersSockets_.clear();
}

}  // namespace Tanks::model
