#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "game_map.h"
#include "movable_entity.h"

namespace Tanks::model {
class Tank : public MovableEntity {
public:
    explicit Tank(int left,
                  int top,
                  EntityType type_,
                  Direction dir,
                  std::unique_ptr<BasicHandler> handler_);

protected:
    void shoot();
};

class BotTank : public Tank {
    friend GameModel;

public:
    explicit BotTank(int left,
                     int top,
                     Direction dir,
                     std::unique_ptr<BasicHandler> handler_);
};

}  // namespace Tanks::model

#endif  // TANKS_TANK_H
