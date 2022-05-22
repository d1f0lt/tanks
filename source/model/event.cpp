#include "model/event.h"
#include <boost/asio.hpp>
#include <cinttypes>
#include <functional>
#include <istream>
#include <ostream>
#include <unordered_map>
#include <vector>

namespace Tanks::model {

namespace {

template <typename T>
void writeInt(boost::asio::ip::tcp::socket &os, const T &a) {
    auto tmp = static_cast<std::int32_t>(a);
    os.write_some(
        boost::asio::buffer(reinterpret_cast<const char *>(&tmp), sizeof(T)));
}

std::int32_t readInt(boost::asio::ip::tcp::socket &is) {
    static std::int32_t buff = 0;
    boost::asio::read(
        is, boost::asio::buffer(reinterpret_cast<char *>(&buff), sizeof(buff)));
    return buff;
}
}  // namespace

void TankMove::sendTo(boost::asio::ip::tcp::socket &os,
                      int id,
                      Direction direction,
                      int speed) {
    writeInt(os, EventType::TANK_MOVE);
    writeInt(os, id);
    writeInt(os, direction);
    writeInt(os, speed);
}

std::unique_ptr<Event> TankMove::readFrom(boost::asio::ip::tcp::socket &is) {
    auto id = readInt(is);
    auto direction = static_cast<Direction>(readInt(is));
    auto speed = readInt(is);
    return std::make_unique<TankMove>(id, direction, speed);
}

TankMove::TankMove(int id, Direction direction, int speed)
    : id_(id), direction_(direction), speed_(speed) {
}

void TankMove::acceptExecutor() {
    return;
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

std::unique_ptr<Event> readEvent(boost::asio::ip::tcp::socket &is) {
    const std::unordered_map<EventType, std::unique_ptr<Event> (*)(
                                            boost::asio::ip::tcp::socket &)>
        readers = []() {
            std::unordered_map<EventType, std::unique_ptr<Event> (*)(
                                              boost::asio::ip::tcp::socket &)>
                tmp;
            tmp[EventType::TANK_MOVE] = TankMove::readFrom;
            return tmp;
        }();

    auto type = static_cast<EventType>(readInt(is));
    return readers.at(type)(is);
};

}  // namespace Tanks::model
