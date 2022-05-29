#include "model/event.h"
#include <unordered_map>
#include "model/network_utils.h"

namespace Tanks::model {
void TankMove::sendTo(boost::asio::ip::tcp::socket &socket,
                      int tankId,
                      Direction direction,
                      int speed) {
    sendInt(socket, EventType::TANK_MOVE);

    sendInt(socket, tankId);
    sendInt(socket, direction);
    sendInt(socket, speed);
}

std::unique_ptr<Event> TankMove::readFrom(
    boost::asio::ip::tcp::socket &socket) {
    auto tankId = receiveInt(socket);
    auto direction = static_cast<Direction>(receiveInt(socket));
    auto speed = receiveInt(socket);

    return std::make_unique<TankMove>(tankId, direction, speed);
}

TankMove::TankMove(int tankId, Direction direction, int speed)
    : id_(tankId), direction_(direction), speed_(speed) {
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

void TankMove::sendTo(boost::asio::ip::tcp::socket &socket) {
    TankMove::sendTo(socket, id_, direction_, speed_);
}

std::unique_ptr<Event> readEvent(boost::asio::ip::tcp::socket &socket) {
    static const std::unordered_map<EventType,
                                    std::unique_ptr<Event> (*)(tcp::socket &)>
        readers = []() {
            std::unordered_map<EventType,
                               std::unique_ptr<Event> (*)(tcp::socket &)>
                tmp;
            tmp[EventType::TANK_MOVE] = TankMove::readFrom;
            tmp[EventType::SPAWN_TANK] = SpawnTank::readFrom;
            return tmp;
        }();

    auto type = static_cast<EventType>(receiveInt(socket));
    assert(static_cast<unsigned>(type) != 0);
    return readers.at(type)(socket);
};

void SpawnTank::sendTo(boost::asio::ip::tcp::socket &socket) {
    static_assert(static_cast<std::int32_t>(EventType::SPAWN_TANK) == 2);
    SpawnTank::sendTo(socket, tankId_, left_, top_, entityType_);
}

void SpawnTank::acceptExecutor(const EventExecutor &executor) {
    executor.execute(*this);
}

void SpawnTank::sendTo(boost::asio::ip::tcp::socket &socket,
                       int tankId,
                       int left,
                       int top,
                       EntityType entityType) {
    sendInt(socket, EventType::SPAWN_TANK);
    sendInt(socket, tankId);
    sendInt(socket, left);
    sendInt(socket, top);
    sendInt(socket, entityType);
}

std::unique_ptr<Event> SpawnTank::readFrom(tcp::socket &socket) {
    auto tankId = receiveInt(socket);
    auto left = receiveInt(socket);
    auto top = receiveInt(socket);
    auto type = static_cast<EntityType>(receiveInt(socket));
    return std::make_unique<SpawnTank>(tankId, left, top, type);
}
EntityType SpawnTank::getType() const {
    return entityType_;
}

SpawnTank::SpawnTank(int tankId, int left, int top, EntityType entityType)
    : tankId_(tankId), left_(left), top_(top), entityType_(entityType) {
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

}  // namespace Tanks::model
