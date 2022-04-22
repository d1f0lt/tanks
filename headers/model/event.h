#ifndef TANKS_EVENT_H
#define TANKS_EVENT_H

#include "tank.h"

namespace Tanks::model {

class Event {
public:
    virtual void execute(GameModel &model) = 0;
    virtual ~Event() = default;
};

}  // namespace Tanks::model

#endif  // TANKS_EVENT_H
