#ifndef TANKS_EVENT_H
#define TANKS_EVENT_H

#include <boost/asio/ip/tcp.hpp>
#include <cinttypes>
#include <iosfwd>
#include <memory>
#include "model/entity.h"
#include "model/event_executor.h"

namespace Tanks::model {
using boost::asio::ip::tcp;

enum class EventType { TANK_MOVE = 1, SPAWN_TANK = 2 };

class Event {
public:
    virtual ~Event() = default;

    virtual void acceptExecutor(const EventExecutor &executor) = 0;
    virtual void sendTo(tcp::socket &os) = 0;
};

class TankMove : public Event {
public:
    explicit TankMove(int id, Direction direction, int speed);

    void acceptExecutor(const EventExecutor &executor) final;
    void sendTo(tcp::socket &os) override;
    static void sendTo(tcp::socket &os, int id, Direction direction, int speed);

    static std::unique_ptr<Event> readFrom(tcp::socket &is);

    [[nodiscard]] int getId() const;
    [[nodiscard]] Direction getDirection() const;
    [[nodiscard]] int getSpeed() const;

private:
    const int id_;
    const Direction direction_;
    const int speed_;
};

class SpawnTank : public Event {
public:
    explicit SpawnTank(int tankId, Direction direction, int type);
    void sendTo(tcp::socket &os) override;
    void acceptExecutor(const EventExecutor &executor) override;

    static void sendTo(tcp::socket &os,
                       int tankId,
                       Direction direction,
                       int type);

    [[nodiscard]] static std::unique_ptr<Event> readFrom(tcp::socket &socket);

private:
    const int tankId_;
    const Direction direction_;
    const int type_;
};

std::unique_ptr<Event> readEvent(tcp::socket &is);

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
