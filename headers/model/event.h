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

enum class EventType {
    TANK_MOVE = 1,
    SPAWN_TANK = 2,
    BONUS_SPAWN = 3,
    SET_POSITION = 4,
    TANK_SHOOT = 5
};

[[nodiscard]] std::unique_ptr<Event> readEvent(tcp::socket &socket);

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

    [[nodiscard]] static std::unique_ptr<Event> readFrom(tcp::socket &socket);

    [[nodiscard]] DecrId getTankId() const;
    [[nodiscard]] int getLeft() const;
    [[nodiscard]] int getTop() const;
    [[nodiscard]] EntityType getType() const;
    [[nodiscard]] EntityType getEntityType() const;
    [[nodiscard]] int getTankSpeed() const;
    [[nodiscard]] int getBulletSpeed() const;
    [[nodiscard]] int getReloadTicks() const;

private:
    const DecrId tankId_;
    const int left_;
    const int top_;
    const EntityType entityType_;
    const int tankSpeed_;
    const int bulletSpeed_;
    const int reloadTicks_;
};

class TankShoot : public Event {
public:
    TankShoot(const int tankId, const Direction direction);

    void acceptExecutor(const EventExecutor &executor) override;

    void sendTo(tcp::socket &socket) override;
    [[nodiscard]] static std::unique_ptr<Event> readFrom(tcp::socket &socket);

    [[nodiscard]] int getTankId() const;
    [[nodiscard]] Direction getDirection() const;

private:
    const int tankId_;
    const Direction direction_;
};

class BonusSpawn : public Event {
public:
    explicit BonusSpawn(int id, int left, int top, EntityType type);

    void sendTo(tcp::socket &socket) override;
    [[nodiscard]] static std::unique_ptr<Event> readFrom(tcp::socket &socket);

    void acceptExecutor(const EventExecutor &executor) override;

    [[nodiscard]] DecrId getId() const;
    [[nodiscard]] int getLeft() const;
    [[nodiscard]] int getTop() const;
    [[nodiscard]] EntityType getType() const;

private:
    const DecrId id_;
    const int left_;
    const int top_;
    const EntityType type_;
};

class SetPosition : public Event {
public:
    explicit SetPosition(int id, int left, int top);

    void acceptExecutor(const EventExecutor &executor) override;
    void sendTo(tcp::socket &socket) override;
    [[nodiscard]] static std::unique_ptr<Event> readFrom(tcp::socket &socket);

    [[nodiscard]] int getId() const;
    [[nodiscard]] int getLeft() const;
    [[nodiscard]] int getTop() const;

private:
    const int id_;
    const int left_;
    const int top_;
};

}  // namespace Tanks::model

#endif  // TANKS_EVENT_H
