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
      socket_(std::make_shared<tcp::socket>(std::move(socket))),
      events_(std::make_shared<SafeQueue<std::unique_ptr<Event>>>()),
      tickSize_(std::make_shared<SafeQueue<int>>())
/*,
       receiver_([&]() { receiveEvents(); }) */
{
    std::thread([&]() { receiveEvents(); }).detach();
};

PlayerActionsHandler ClientModel::getHandler() {
    return PlayerActionsHandler(playerId_, *this, *socket_);
}

// TODO: no copypast
void ClientModel::addEvent(std::unique_ptr<Event> event) {
    events_->Produce(std::move(event));
}

void ClientModel::receiveEvents() {
    auto tickSize = atomic_load(&tickSize_);
    auto events = atomic_load(&events_);
    auto socket = atomic_load(&socket_);
    try {
        while (true) {
            int count = receiveInt(*socket);
            tickSize->Produce(count);
            for (int i = 0; i < count; i++) {
                events->Produce(readEvent(*socket));
            }
#ifndef NDEBUG
            static std::fstream log;
            log.open("log_client.txt");
            log << count << std::endl;
            log.close();
#endif
        }
    } catch (boost::system::system_error &) {
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
    int ticks = std::max(1, static_cast<int>(tickSize_->Size()));
    for (; ticks > 0 && !isFinished(); ticks--) {
        if (!tickSize_->ConsumeSync(cnt)) {
            return;
        }
        for (; cnt > 0; cnt--) {
            bool res = events_->ConsumeSync(event);
            assert(res);
            executeEvent(*event);
            if (dynamic_cast<GameEnd *>(event.get())) {
                return;
            }
        }
    }
}

ClientModel::~ClientModel() noexcept {
    //    socket_.close();
    //        receiver_.join();
}

std::optional<std::reference_wrapper<Tank>> ClientModel::tank() {
    auto entity = getById(playerId_);
    if (!entity) {
        return std::nullopt;
    }
    return dynamic_cast<Tank &>(entity->get());
}

void ClientModel::finishGame() {
    setFinished();
    //    socket_->close();
    //    receiver_.join();
}

}  // namespace Tanks::model
