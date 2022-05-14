#ifndef TANKS_PLAYABLE_TANK_H
#define TANKS_PLAYABLE_TANK_H

#include <memory>
#include <queue>
#include "model/tank.h"

namespace Tanks::model {
class PlayableTank : public Tank {
public:
    explicit PlayableTank(int left,
                          int top,
                          int entityId,
                          std::unique_ptr<TankHandler> handler,
                          Direction direction);

    explicit PlayableTank(int left,
                          int top,
                          int entityId,
                          Direction direction,
                          GameModel &model);

    [[nodiscard]] EntityType getType() const override;

    using MovableEntity::move;
    using MovableEntity::setDirection;
    using Tank::shoot;

    void move(Direction direction);
};
}  // namespace Tanks::model
#endif  // TANKS_PLAYABLE_TANK_H
