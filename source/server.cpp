#include "server.h"
#include <chrono>
#include <thread>
#include "model/network_utils.h"

namespace Tanks {
using boost::asio::ip::tcp;
using Menu::PlayerSkills;
Server::Server(const std::string &levelFilename,
               int bots,
               int bonuses,
               int level)
    : /*isStarted(std::make_unique<std::atomic<bool>>(false)),
      isStopped(std::make_unique<std::atomic<bool>>(false)),
      ioContext_(std::make_shared<boost::asio::io_context>()),*/
      acceptor_(ioContext_, tcp::endpoint(tcp::v4(), 0)),
      model_(
          std::make_unique<model::ServerModel>(levelFilename, bots, bonuses)),
      level_(level) {
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
        sockets_.emplace_back(std::move(socket));
        assert(sockets_.size() <= 20);
        PlayerSkills skills = receiveFrom(sockets_.back());
        int lives = model::receiveInt(sockets_.back());

        int id = model_->addPlayer(sockets_.back(), skills, lives);
        model::sendInt(sockets_.back(), id);
        model::sendInt(sockets_.back(), getLevel());
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
}  // namespace Tanks
