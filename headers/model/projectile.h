#ifndef TANKS_PROJECTILE_H
#define TANKS_PROJECTILE_H

#include "movable_entity.h"

namespace Tanks::model {
class Projectile : public MovableEntity {
public:
    explicit Projectile(int left,
                        int top,
                        Direction direction,
                        std::unique_ptr<ProjectileHandler> handler);

    explicit Projectile(int left,
                        int top,
                        Direction direction,
                        GameModel &model);

    [[nodiscard]] EntityType getType() const override;
    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;
    [[nodiscard]] int getStrength() const override;
    [[nodiscard]] int getSpeed() const override;

    [[nodiscard]] bool canPass(const Entity &other) const override;
};
}  // namespace Tanks::model

#endif  // TANKS_PROJECTILE_H
