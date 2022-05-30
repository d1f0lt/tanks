#include "model/client_game_model.h"
#include <thread>
#include "model/network_utils.h"
#include "model/tank.h"
#ifndef NDEBUG
#include <fstream>
#include <iostream>
#endif

namespace Tanks::model {
ClientModel::ClientModel(int playerId, tcp::socket socket)
    : playerId_(playerId),
      socket_(std::move(socket)),
      receiver([&]() { receiveEvents(); }){
          //    std::thread([&]() { receiveEvents(); }).detach();
      };

PlayerActionsHandler ClientModel::getHandler() {
    return PlayerActionsHandler(playerId_, socket_);
}

// TODO: no copypast
void ClientModel::addEvent(std::unique_ptr<Event> event) {
    events_.Produce(std::move(event));
}

void ClientModel::receiveEvents() {
    try {
        while (true) {
            int count = receiveInt(socket_);
            if (count == -1) {
                return;
            }
            {
                int countCopy = count;
                tickSize_.Produce(std::move(countCopy));
            }
            for (int i = 0; i < count; i++) {
                events_.Produce(readEvent(socket_));
            }
#ifndef NDEBUG
            static std::fstream log;
            log.open("log_client.txt");
            log << count << std::endl;
            log.close();
#endif
        }
    } catch (boost::system::system_error &e) {
        return;
    } catch (std::system_error &e) {
#ifndef NDEBUG
        static std::fstream log("log_client.txt");
        log << getTick() << ' ' << e.what() << std::endl;
#endif
        return;
    }
}

void ClientModel::executeAllEvents() {
    std::unique_ptr<Event> event;
    int cnt = -1;
    if (!tickSize_.ConsumeSync(cnt)) {
        assert(false);
    }
    for (; cnt > 0; cnt--) {
        bool res = events_.ConsumeSync(event);
        assert(res);
        executeEvent(*event);
    }
}

ClientModel::~ClientModel() noexcept {
    socket_.close();
    receiver.join();
}

std::optional<std::reference_wrapper<Tank>> ClientModel::tank() {
    auto entity = getById(playerId_);
    if (!entity) {
        return std::nullopt;
    }
    return dynamic_cast<Tank &>(entity->get());
}

}  // namespace Tanks::model
