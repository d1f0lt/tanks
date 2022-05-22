#ifndef TANKS_EVENT_H
#define TANKS_EVENT_H

#include <boost/asio/ip/tcp.hpp>
#include <cinttypes>
#include <iosfwd>
#include <memory>
#include "model/entity.h"

namespace Tanks::model {
enum class EventType { TANK_MOVE = 1 };

class Event {
public:
    virtual ~Event() = default;

    virtual void acceptExecutor() = 0;
    virtual void sendTo(boost::asio::ip::tcp::socket &os) = 0;
};

class TankMove : public Event {
public:
    explicit TankMove(int id, Direction direction, int speed);

    void acceptExecutor() final;
    void sendTo(boost::asio::ip::tcp::socket &os) override;
    static void sendTo(boost::asio::ip::tcp::socket &os,
                       int id,
                       Direction direction,
                       int speed);

    static std::unique_ptr<Event> readFrom(boost::asio::ip::tcp::socket &is);

    [[nodiscard]] int getId() const;
    [[nodiscard]] Direction getDirection() const;
    [[nodiscard]] int getSpeed() const;

private:
    const int id_;
    const Direction direction_;
    const int speed_;
};

std::unique_ptr<Event> readEvent(boost::asio::ip::tcp::socket &is);

// class BonusSpawn : public Event {
// public:
//     void acceptExecutor() final;
//
//     static void sendTo();
//     static std::unique_ptr<BonusSpawn> readFrom();
//
// private:
//     int id_;
//     int left_;
//     int right_;
//     EntityType type_;
// };

}  // namespace Tanks::model

#endif  // TANKS_EVENT_H
