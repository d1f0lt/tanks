#ifndef TANKS_SERVER_H
#define TANKS_SERVER_H

#include <boost/asio.hpp>
#include "model/server_game_model.h"

namespace Tanks {
using boost::asio::ip::tcp;
class Server {
public:
    explicit Server(const std::string &levelFilename, int bots, int bonuses);

    std::thread start();

    void stop();

    [[nodiscard]] tcp::endpoint getEndpoint();

private:
    std::atomic<bool> isStarted = false;
    std::atomic<bool> isStopped = false;
    std::mutex mutex_;
    std::vector<boost::asio::ip::tcp::socket> sockets_;
    model::ServerModel model_;
    boost::asio::io_context ioContext_;
    tcp::acceptor acceptor_;
    tcp::endpoint endpoint_;

    void listenForNewPlayers();

    void work();
};
}  // namespace Tanks

#endif  // TANKS_SERVER_H
