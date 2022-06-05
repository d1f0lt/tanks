#include "server.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "model/network_utils.h"

namespace Tanks {
using boost::asio::ip::tcp;
using Menu::PlayerSkills;
Server::Server(const std::string &levelFilename,
               int players,
               int bots,
               int bonuses,
               int level)
    : /*isStarted(std::make_unique<std::atomic<bool>>(false)),
      isStopped(std::make_unique<std::atomic<bool>>(false)),
      ioContext_(std::make_shared<boost::asio::io_context>()),*/
      acceptor_(ioContext_, tcp::endpoint(tcp::v4(), 0)),
      model_(
          std::make_unique<model::ServerModel>(levelFilename, bots, bonuses)),
      level_(level),
      players_(players),
      bots_(bots),
      bonuses_(bonuses) {
    sockets_.reserve(100);
}

// std::thread Server::start() {
//  TODO notify all
//    return std::thread([&]() { work(); });
//}

void Server::stop() {
    isStopped = true;
}

tcp::endpoint Server::getEndpoint() {
    return acceptor_.local_endpoint();
}

PlayerSkills receiveFrom(tcp::socket &socket) {
    auto [tankSpeed, reloadTick, bulletSpeed] =
        model::receiveMultipleInts<int, int, int>(socket);
    return {tankSpeed, reloadTick, bulletSpeed};
}

void Server::listenForNewPlayer() {
    try {
        auto socket = acceptor_.accept();
        socket.set_option(tcp::no_delay(true));

        sockets_.emplace_back(std::move(socket));
        assert(sockets_.size() <= 20);
        PlayerSkills skills = receiveFrom(sockets_.back());
        int lives = model::receiveInt(sockets_.back());

        int id = model_->addPlayer(sockets_.back(), skills, lives);
        model::sendInt(sockets_.back(), id);
        model::sendInt(sockets_.back(), getLevel());
        model::sendMultipleInts(sockets_.back(), players_, bots_, bonuses_);
    } catch (boost::system::system_error &) {
    }
}

void Server::start() {
    isStarted = true;
}

void Server::work() {
    while (!isStopped) {
        model_->nextTick();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

bool Server::getIsStopped() const {
    return isStopped;
}
void Server::nextTick() {
    model_->nextTick();
}

bool Server::getIsStarted() const {
    return isStarted;
}

int Server::getLevel() const {
    return level_;
}

[[noreturn]] void createAndRunServer(int level,
                                     int players,
                                     int bots,
                                     int bonuses) {
    const std::string levelFilename("../levels/level" + std::to_string(level) +
                                    ".csv");
    Server server(levelFilename, players, bots, bonuses, level);
    std::cout << server.getEndpoint() << std::endl;
    for (int i = 0; i < players; i++) {
        server.listenForNewPlayer();
        std::cout << i + 1 << ' ' << "connected" << std::endl;
    }

    std::cout << "All connected!" << std::endl;

    while (true) {
        server.nextTick();
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}
}  // namespace Tanks
