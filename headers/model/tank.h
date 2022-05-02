#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "game_map.h"
#include "movable_entity.h"

namespace Tanks::model {
class Tank : public MovableEntity {
public:
    explicit Tank(int left_,
                  int top_,
                  EntityType type_,
                  Direction direction_,
                  std::unique_ptr<BasicHandler> handler_);

    explicit Tank(int left_,
                  int top_,
                  EntityType type_,
                  Direction direction_,
                  GameModel &model_);

protected:
    void shoot();
};

class BotTank : public Tank {
public:
    explicit BotTank(int left_,
                     int top_,
                     Direction direction_,
                     std::unique_ptr<BasicHandler> handler_);
};

}  // namespace Tanks::model

#endif  // TANKS_TANK_H
