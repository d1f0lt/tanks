#ifndef TANKS_PLAYABLE_TANK_H
#define TANKS_PLAYABLE_TANK_H

#include <memory>
#include <queue>
#include "model/tank.h"

namespace Tanks::model {
class PlayableTank : public Tank {
public:
    explicit PlayableTank(int left_,
                          int top_,
                          Direction direction_,
                          std::unique_ptr<BasicHandler> handler_);

    explicit PlayableTank(int left,
                          int top,
                          int id,
                          std::unique_ptr<BasicHandler> handler,
                          Direction direction,
                          int speed);

    explicit PlayableTank(int left, int top, Direction dir, GameModel &model);
    [[nodiscard]] EntityType getType() const override;

    using MovableEntity::move;
    using MovableEntity::setDirection;
    using Tank::shoot;
};
}  // namespace Tanks::model
#endif  // TANKS_PLAYABLE_TANK_H
