#include "server.h"
#include <boost/asio.hpp>
#include <chrono>
#include <thread>
#include "model/network_utils.h"

namespace Tanks {
using boost::asio::ip::tcp;
Server::Server(const std::string &levelFilename, int bots, int bonuses)
    : /*isStarted(std::make_unique<std::atomic<bool>>(false)),
      isStopped(std::make_unique<std::atomic<bool>>(false)),
      ioContext_(std::make_shared<boost::asio::io_context>()),*/
      acceptor_(ioContext_, tcp::endpoint(tcp::v4(), 0)),
      model_(
          std::make_unique<model::ServerModel>(levelFilename, bots, bonuses)) {
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

void Server::listenForNewPlayer() {
    try {
        // TODO get skills
        auto socket = acceptor_.accept();
        sockets_.emplace_back(std::move(socket));
        assert(sockets_.size() <= 20);

        int id = model_->addPlayer(sockets_.back(), {});
        model::sendInt(sockets_.back(), id);
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
}  // namespace Tanks
