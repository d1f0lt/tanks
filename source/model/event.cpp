#include "model/event.h"
#include <cinttypes>
#include <functional>
#include <istream>
#include <ostream>
#include <unordered_map>
#include <vector>
#include "model/network_utils.h"

namespace Tanks::model {

namespace {}  // namespace

void TankMove::sendTo(boost::asio::ip::tcp::socket &os,
                      int id,
                      Direction direction,
                      int speed) {
    sendInt(os, EventType::TANK_MOVE);

    sendInt(os, id);
    sendInt(os, direction);
    sendInt(os, speed);
}

std::unique_ptr<Event> TankMove::readFrom(boost::asio::ip::tcp::socket &is) {
    auto id = receiveInt(is);
    auto direction = static_cast<Direction>(receiveInt(is));
    auto speed = receiveInt(is);

    return std::make_unique<TankMove>(id, direction, speed);
}

TankMove::TankMove(int id, Direction direction, int speed)
    : id_(id), direction_(direction), speed_(speed) {
}

void TankMove::acceptExecutor(const EventExecutor &executor) {
    executor.execute(*this);
}

int TankMove::getId() const {
    return id_;
}

Direction TankMove::getDirection() const {
    return direction_;
}

int TankMove::getSpeed() const {
    return speed_;
}

void TankMove::sendTo(boost::asio::ip::tcp::socket &os) {
    TankMove::sendTo(os, id_, direction_, speed_);
}

std::unique_ptr<Event> readEvent(boost::asio::ip::tcp::socket &socket) {
    const std::unordered_map<EventType, std::unique_ptr<Event> (*)(
                                            boost::asio::ip::tcp::socket &)>
        readers = []() {
            std::unordered_map<EventType, std::unique_ptr<Event> (*)(
                                              boost::asio::ip::tcp::socket &)>
                tmp;
            tmp[EventType::TANK_MOVE] = TankMove::readFrom;
            tmp[EventType::SPAWN_TANK] = SpawnTank::readFrom;
            return tmp;
        }();

    auto type = static_cast<EventType>(receiveInt(socket));
    return readers.at(type)(socket);
};

void SpawnTank::sendTo(boost::asio::ip::tcp::socket &socket) {
    SpawnTank::sendTo(socket, tankId_, left_, top_, direction_);
}

void SpawnTank::acceptExecutor(const EventExecutor &executor) {
    executor.execute(*this);
}

void SpawnTank::sendTo(boost::asio::ip::tcp::socket &socket,
                       int tankId,
                       int left,
                       int top,
                       Direction direction) {
    sendInt(socket, tankId);
    sendInt(socket, left);
    sendInt(socket, top);
    sendInt(socket, direction);
}

std::unique_ptr<Event> SpawnTank::readFrom(tcp::socket &socket) {
    auto tankId = receiveInt(socket);
    auto left = receiveInt(socket);
    auto top = receiveInt(socket);
    auto direction = static_cast<Direction>(receiveInt(socket));
    return std::make_unique<SpawnTank>(tankId, left, top, direction);
}

SpawnTank::SpawnTank(int tankId, int left, int top, Direction direction)
    : tankId_(tankId), left_(left), top_(top), direction_(direction) {
}
int SpawnTank::getTankId() const {
    return tankId_;
}
int SpawnTank::getLeft() const {
    return left_;
}

int SpawnTank::getTop() const {
    return top_;
}

Direction SpawnTank::getDirection() const {
    return direction_;
}

}  // namespace Tanks::model
