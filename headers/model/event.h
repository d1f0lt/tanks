#ifndef TANKS_EVENT_H
#define TANKS_EVENT_H

#include "model/tank.h"

namespace Tanks::model {
class GameModel;  // TODO forward declaration

class Event {
    virtual void execute(GameModel &model) = 0;
};

class TankMove : public Event {
public:
    void execute(GameModel &model) final;

private:
    PlayableTank &Tank;
    Tanks::model::Direction direction;
};
}  // namespace Tanks::model

#endif  // TANKS_EVENT_H
