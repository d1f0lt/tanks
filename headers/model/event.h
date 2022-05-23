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
    explicit SpawnTank(int tankId, int left, int top, Direction direction);

    void sendTo(tcp::socket &socket) override;
    void acceptExecutor(const EventExecutor &executor) override;

    static void sendTo(tcp::socket &socket,
                       int tankId,
                       int left,
                       int top,
                       Direction direction);

    [[nodiscard]] static std::unique_ptr<Event> readFrom(tcp::socket &socket);

    int getTankId() const;
    int getLeft() const;
    int getTop() const;
    Direction getDirection() const;

private:
    const int tankId_;
    const int left_;
    const int top_;
    const Direction direction_;
};

std::unique_ptr<Event> readEvent(tcp::socket &socket);

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
