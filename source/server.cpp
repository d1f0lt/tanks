#include "server.h"
#include <boost/asio.hpp>
#include <chrono>
#include <thread>
#include "model/network_utils.h"

namespace Tanks {
using boost::asio::ip::tcp;
Server::Server(const std::string &levelFilename, int bots, int bonuses)
    : model_(levelFilename, bots, bonuses),
      acceptor_(ioContext_, tcp::endpoint(tcp::v4(), 0)),
      endpoint_(acceptor_.local_endpoint()) {
    sockets_.reserve(20);
    //    acceptor_ = tcp::acceptor(ioContext_);
    std::thread([&]() { listenForNewPlayers(); }).detach();
}

std::thread Server::start() {
    isStarted = true;
    // TODO notify all
    return std::thread([&]() { work(); });
}

void Server::stop() {
    isStopped = true;
    //    model_.finishGame();
}

tcp::endpoint Server::getEndpoint() {
    return endpoint_;
}

void Server::listenForNewPlayers() {
    try {
        while (!isStarted) {
            // TODO get skills
            sockets_.emplace_back(acceptor_.accept());
            assert(sockets_.size() <= 20);

            int id = model_.addPlayer(sockets_.back(), {});
            model::sendInt(sockets_.back(), id);
        }
    } catch (boost::system::system_error &) {
    }
}

void Server::work() {
    while (!isStopped) {
        model_.nextTick();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
}  // namespace Tanks
