#ifndef TANKS_SERVER_H
#define TANKS_SERVER_H

#include <boost/asio.hpp>
#include "model/server_game_model.h"

namespace Tanks {
using boost::asio::ip::tcp;
class Server {
public:
    explicit Server(const std::string &levelFilename, int bots, int bonuses);

    //    std::thread start();

    void stop();
    void start();

    [[nodiscard]] tcp::endpoint getEndpoint();

    void listenForNewPlayer();
    [[nodiscard]] bool getIsStopped() const;

    void nextTick();

private:
    std::unique_ptr<std::atomic<bool>> isStarted;
    std::unique_ptr<std::atomic<bool>> isStopped;
    std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>> sockets_;
    std::unique_ptr<model::ServerModel> model_;
    std::shared_ptr<boost::asio::io_context> ioContext_;
    tcp::acceptor acceptor_;

    void work();
};
}  // namespace Tanks

#endif  // TANKS_SERVER_H
