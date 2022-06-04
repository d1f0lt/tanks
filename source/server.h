#ifndef TANKS_SERVER_H
#define TANKS_SERVER_H

#include <boost/asio.hpp>
#include "model/server_game_model.h"

namespace Tanks {
using boost::asio::ip::tcp;
class Server {
public:
    explicit Server(const std::string &levelFilename,
                    int bots,
                    int bonuses,
                    int level);

    void stop();
    void start();

    [[nodiscard]] tcp::endpoint getEndpoint();
    [[nodiscard]] bool getIsStopped() const;
    [[nodiscard]] bool getIsStarted() const;
    [[nodiscard]] int getLevel() const;

    void listenForNewPlayer();

    void nextTick();

private:
    std::atomic<bool> isStarted = false;
    std::atomic<bool> isStopped = false;
    boost::asio::io_context ioContext_;
    tcp::acceptor acceptor_;
    std::vector<tcp::socket> sockets_;
    std::unique_ptr<model::ServerModel> model_;
    const int level_;

    void work();
};
}  // namespace Tanks

#endif  // TANKS_SERVER_H
