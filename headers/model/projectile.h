#ifndef TANKS_PROJECTILE_H
#define TANKS_PROJECTILE_H

#include "model/game_model_fwd.h"
#include "movable_entity.h"

namespace Tanks::model {
class Projectile : public MovableEntity {
public:
    explicit Projectile(int left,
                        int top,
                        Direction direction,
                        GameModel &model,
                        IncrId entityId,
                        int speed);

    [[nodiscard]] EntityType getType() const override;
    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;
    [[nodiscard]] int getStrength() const override;
    [[nodiscard]] int getSpeed() const override;

    [[nodiscard]] bool isTankPassable() const override;

private:
    const int speed_;
};
}  // namespace Tanks::model

#endif  // TANKS_PROJECTILE_H
