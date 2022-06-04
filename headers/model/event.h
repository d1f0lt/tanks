#ifndef TANKS_EVENT_H
#define TANKS_EVENT_H

#include <boost/asio/ip/tcp.hpp>
#include <cinttypes>
#include <iosfwd>
#include <memory>
#include "entity.h"
#include "model/event_executor.h"
#include "player_skills.h"

#ifdef MODEL_LOGS
#include <fstream>
#include "event.h"
#endif

namespace Tanks::model {
using boost::asio::ip::tcp;

enum class EventType {
    TANK_MOVE = 1,
    SPAWN_TANK = 2,
    BONUS_SPAWN = 3,
    SET_POSITION = 4,
    TANK_SHOOT = 5,
    GAME_END = 6,
};

[[nodiscard]] std::unique_ptr<Event> readEvent(tcp::socket &socket);

class Event {
public:
    virtual ~Event() = default;

    virtual bool acceptExecutor(const EventVisitor &executor) = 0;
    virtual void sendTo(tcp::socket &socket) const = 0;
    [[nodiscard]] virtual EventType getType() const = 0;
};

class TankMove : public Event {
public:
    explicit TankMove(int tankId, Direction direction, int speed);

    [[nodiscard]] EventType getType() const override;

    bool acceptExecutor(const EventVisitor &executor) final;
    void sendTo(tcp::socket &socket) const override;

    static std::unique_ptr<Event> readFrom(tcp::socket &socket);

    [[nodiscard]] int getId() const;
    [[nodiscard]] Direction getDirection() const;
    [[nodiscard]] int getSpeed() const;

private:
    const int id_;
    const Direction direction_;
    const int speed_;
};

class TankSpawn : public Event {
public:
    explicit TankSpawn(int tankId,
                       int left,
                       int top,
                       EntityType entityType,
                       Menu::PlayerSkills skills);

    [[nodiscard]] EventType getType() const override;

    void sendTo(tcp::socket &socket) const override;
    bool acceptExecutor(const EventVisitor &executor) override;

    [[nodiscard]] static std::unique_ptr<Event> readFrom(tcp::socket &socket);

    [[nodiscard]] DecrId getTankId() const;
    [[nodiscard]] int getLeft() const;
    [[nodiscard]] int getTop() const;
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
    explicit TankShoot(int tankId, Direction direction);

    [[nodiscard]] EventType getType() const override;

    bool acceptExecutor(const EventVisitor &executor) override;

    void sendTo(tcp::socket &socket) const override;
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

    [[nodiscard]] EventType getType() const override;

    void sendTo(tcp::socket &socket) const override;
    [[nodiscard]] static std::unique_ptr<Event> readFrom(tcp::socket &socket);

    bool acceptExecutor(const EventVisitor &executor) override;

    [[nodiscard]] DecrId getId() const;
    [[nodiscard]] int getLeft() const;
    [[nodiscard]] int getTop() const;
    [[nodiscard]] EntityType getEntityType() const;

private:
    const DecrId id_;
    const int left_;
    const int top_;
    const EntityType type_;
};

class SetPosition : public Event {
public:
    explicit SetPosition(int id, int left, int top);

    [[nodiscard]] EventType getType() const override;

    bool acceptExecutor(const EventVisitor &executor) override;
    void sendTo(tcp::socket &socket) const override;
    [[nodiscard]] static std::unique_ptr<Event> readFrom(tcp::socket &socket);

    [[nodiscard]] int getId() const;
    [[nodiscard]] int getLeft() const;
    [[nodiscard]] int getTop() const;

private:
    const int id_;
    const int left_;
    const int top_;
};

class GameEnd : public Event {
public:
    explicit GameEnd(int kills);

    [[nodiscard]] EventType getType() const override;

    bool acceptExecutor(const EventVisitor &executor) override;

    [[nodiscard]] static std::unique_ptr<Event> readFrom(tcp::socket &socket);
    void sendTo(tcp::socket &socket) const override;

    [[nodiscard]] int getKills() const;

private:
    const int kills_;
};

#ifdef MODEL_LOGS
template <typename T>
static void print(std::ofstream &file, T a) {
    file << static_cast<int>(a) << ' ';
}

template <>
inline void print<EventType>(std::ofstream &file, EventType type) {
    switch (type) {
        case EventType::TANK_MOVE:
            file << "TANK_MOVE ";
            break;
        case EventType::SPAWN_TANK:
            file << "TANK_SPAWN ";
            break;
        case EventType::BONUS_SPAWN:
            file << "BONUS_SPAWN ";
            break;
        case EventType::SET_POSITION:
            file << "SET_POSITION ";
            break;
        case EventType::TANK_SHOOT:
            file << "TANK_SHOOT ";
            break;
        case EventType::GAME_END:
            file << "GAME_END ";
            break;
    }
}

template <>
inline void print<Direction>(std::ofstream &file, Direction direction) {
    switch (direction) {
        case Direction::LEFT:
            file << "LEFT ";
            break;
        case Direction::RIGHT:
            file << "RIGHT ";
            break;
        case Direction::DOWN:
            file << "DOWN ";
            break;
        case Direction::UP:
            file << "UP ";
            break;
    }
}

template <typename... Args>
static void printManyInts(std::ofstream &file, Args... args) {
    (print(file, args), ...);
}

inline void printEvent(const Event &event, std::ofstream &file) {
    switch (event.getType()) {
        case (EventType::TANK_MOVE): {
            auto &move = dynamic_cast<const TankMove &>(event);
            printManyInts(file, move.getType(), move.getId(),
                          move.getDirection(), move.getSpeed());
        } break;
        case EventType::SPAWN_TANK: {
            auto &spawn = dynamic_cast<const TankSpawn &>(event);
            printManyInts(file, spawn.getType(), spawn.getTankId(),
                          spawn.getEntityType(), spawn.getLeft(),
                          spawn.getTop(), spawn.getReloadTicks(),
                          spawn.getBulletSpeed(), spawn.getTankSpeed());
        } break;
        case EventType::BONUS_SPAWN: {
            auto &bonus = dynamic_cast<const BonusSpawn &>(event);
            printManyInts(file, bonus.getType(), bonus.getId(), bonus.getLeft(),
                          bonus.getTop());
        }

        break;
        case EventType::SET_POSITION: {
            //            assert(false);
        } break;
        case EventType::TANK_SHOOT: {
            auto &shoot = dynamic_cast<const TankShoot &>(event);
            printManyInts(file, shoot.getType(), shoot.getTankId(),
                          shoot.getDirection());
        } break;
        case EventType::GAME_END:
            break;
    }
}
#endif
}  // namespace Tanks::model

#endif  // TANKS_EVENT_H
