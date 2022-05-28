#ifndef TANKS_BONUS_H
#define TANKS_BONUS_H

#include "model/foreground_entity.h"
#include "model/handler.h"

namespace Tanks::model {
class Bonus : public ForegroundEntity {
public:
    Bonus(int left,
          int top,
          int entityId,
          std::unique_ptr<BonusHandler> handler);

    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;
    [[nodiscard]] bool canPass(const Entity &other) const override;
};

class WalkOnWater : public Bonus {
public:
    explicit WalkOnWater(int left, int top, int entityId, GameModel &model);

    [[nodiscard]] EntityType getType() const override;
};
}  // namespace Tanks::model

#endif  // TANKS_BONUS_H
