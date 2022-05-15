#ifndef TANKS_EVENT_H
#define TANKS_EVENT_H

#include <cinttypes>
#include <iosfwd>
#include <memory>
#include "model/entity.h"

namespace Tanks::model {
enum class EventType { TANK_MOVE = 1 };

class Event {
public:
    virtual void execute() = 0;

    virtual ~Event() = default;
};

class TankMove : public Event {
public:
    explicit TankMove(int id, Direction direction, int speed);

    void execute() final;

    static void writeTo(std::ostream &os,
                        int id,
                        Direction direction,
                        int speed);

    static std::unique_ptr<Event> readFrom(std::istream &is);

    [[nodiscard]] int getId() const;
    [[nodiscard]] Direction getDirection() const;

private:
    int id_;
    Direction direction_;
    int speed_;
};

std::unique_ptr<Event> readEvent(std::istream &is);

// class BonusSpawn : public Event {
// public:
//     void execute() final;
//
//     static void writeTo();
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
