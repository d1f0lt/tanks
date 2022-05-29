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
    virtual void sendTo(tcp::socket &socket) = 0;
};

class TankMove : public Event {
public:
    explicit TankMove(int tankId, Direction direction, int speed);

    void acceptExecutor(const EventExecutor &executor) final;
    void sendTo(tcp::socket &socket) override;
    static void sendTo(tcp::socket &socket,
                       int tankId,
                       Direction direction,
                       int speed);

    static std::unique_ptr<Event> readFrom(tcp::socket &socket);

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
    explicit SpawnTank(int tankId,
                       int left,
                       int top,
                       EntityType entityType,
                       int tankSpeed,
                       int bulletSpeed,
                       int reloadTicks);

    void sendTo(tcp::socket &socket) override;
    void acceptExecutor(const EventExecutor &executor) override;

    static void sendTo(boost::asio::ip::tcp::socket &socket,
                       int tankId,
                       int left,
                       int top,
                       EntityType entityType,
                       int tankSpeed,
                       int bulletSpeed,
                       int reloadTicks);

    [[nodiscard]] static std::unique_ptr<Event> readFrom(tcp::socket &socket);

    [[nodiscard]] int getTankId() const;
    [[nodiscard]] int getLeft() const;
    [[nodiscard]] int getTop() const;
    [[nodiscard]] EntityType getType() const;
    [[nodiscard]] EntityType getEntityType() const;
    [[nodiscard]] int getTankSpeed() const;
    [[nodiscard]] int getBulletSpeed() const;
    [[nodiscard]] int getReloadTicks() const;

private:
    const int tankId_;
    const int left_;
    const int top_;
    const EntityType entityType_;
    const int tankSpeed_;
    const int bulletSpeed_;
    const int reloadTicks_;
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
