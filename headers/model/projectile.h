#ifndef TANKS_PROJECTILE_H
#define TANKS_PROJECTILE_H

#include "movable_entity.h"

namespace Tanks::model {
class Projectile : public MovableEntity {
public:
    explicit Projectile(int left_,
                        int top_,
                        Direction direction_,
                        std::unique_ptr<BasicHandler> handler_);

    explicit Projectile(int left_,
                        int top_,
                        Direction direction_,
                        GameModel &model_);

    [[nodiscard]] EntityType getType() const override;
    [[nodiscard]] int getStrength() const override;
    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;
};
}  // namespace Tanks::model

#endif  // TANKS_PROJECTILE_H
