#ifndef TANKS_EVENT_H
#define TANKS_EVENT_H

#include "model/entity.h"
#include "tank.h"

namespace Tanks::model {

class Event {
public:
    virtual void execute(GameModel &model) = 0;
    virtual ~Event() = default;
};

class TankMove : public Event {
public:
    explicit TankMove(Tank &tank_, Direction direction_);
    void execute(GameModel &model) final;

private:
    Tank &tank;
    Direction direction;
};
}  // namespace Tanks::model

#endif  // TANKS_EVENT_H
