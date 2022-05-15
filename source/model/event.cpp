#include "model/event.h"
#include <cinttypes>
#include <functional>
#include <istream>
#include <ostream>
#include <unordered_map>
#include <vector>

namespace Tanks::model {

namespace {

template <typename T>
void writeInt(std::ostream &os, const T &a) {
    auto tmp = static_cast<std::int32_t>(a);
    os.write(reinterpret_cast<const char *>(&tmp), sizeof(T));
}

std::int32_t readInt(std::istream &is) {
    static std::int32_t buff = 0;
    is.readsome(reinterpret_cast<char *>(&buff), sizeof(buff));
    return buff;
}
}  // namespace

void TankMove::writeTo(std::ostream &os,
                       int id,
                       Direction direction,
                       int speed) {
    writeInt(os, EventType::TANK_MOVE);
    writeInt(os, id);
    writeInt(os, direction);
    writeInt(os, speed);
}

std::unique_ptr<Event> TankMove::readFrom(std::istream &is) {
    auto id = readInt(is);
    auto direction = static_cast<Direction>(readInt(is));
    auto speed = readInt(is);
    return std::make_unique<TankMove>(id, direction, speed);
}

TankMove::TankMove(int id, Direction direction, int speed)
    : id_(id), direction_(direction), speed_(speed) {
}

void TankMove::execute() {
    return;
}

int TankMove::getId() const {
    return id_;
}

Direction TankMove::getDirection() const {
    return direction_;
}

std::unique_ptr<Event> readEvent(std::istream &is) {
    const std::unordered_map<EventType,
                             std::unique_ptr<Event> (*)(std::istream &)>
        readers = []() {
            std::unordered_map<EventType,
                               std::unique_ptr<Event> (*)(std::istream &)>
                tmp;
            tmp[EventType::TANK_MOVE] = TankMove::readFrom;
            return tmp;
        }();

    int a;
    is >> a;
    auto type = static_cast<EventType>(readInt(is));
    return readers.at(type)(is);
};

}  // namespace Tanks::model
