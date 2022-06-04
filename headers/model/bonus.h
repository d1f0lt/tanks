#ifndef TANKS_BONUS_H
#define TANKS_BONUS_H

#include "foreground_entity.h"
#include "model/handler.h"

namespace Tanks::model {
class Bonus : public ForegroundEntity {
public:
    Bonus(int left,
          int top,
          DecrId entityId,
          std::unique_ptr<BonusHandler> handler);

    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;
    [[nodiscard]] int getStrength() const override;
    [[nodiscard]] bool isTankPassable() const override;
    [[nodiscard]] bool isBulletPassable() const override;

    void apply(Tank &tank);
};

class WalkOnWater : public Bonus {
public:
    explicit WalkOnWater(int left, int top, DecrId entityId, GameModel &model);

    [[nodiscard]] EntityType getType() const override;
};
}  // namespace Tanks::model

#endif  // TANKS_BONUS_H
