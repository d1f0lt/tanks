#ifndef TANKS_TANK_MOVE_EVENT_H
#define TANKS_TANK_MOVE_EVENT_H

#include "model/event.h"
#include "model/tank.h"

namespace Tanks::model {
class TankMove : public Event {
public:
    explicit TankMove(Tank &tank_, Direction direction_);
    void execute(GameModel &model) final;

private:
    Tank &tank;
    Direction direction;
};
}  // namespace Tanks::model
#endif  // TANKS_TANK_MOVE_EVENT_H
