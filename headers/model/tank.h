#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "game_model_fwd.h"
#include "movable_entity.h"

namespace Tanks::model {
class Tank : public MovableEntity {
public:
    explicit Tank(int left,
                  int top,
                  int entityId,
                  std::unique_ptr<TankHandler> handler,
                  Direction direction,
                  int speed);

    [[nodiscard]] bool canPass(const Entity &other) const override;
    [[nodiscard]] int getSpeed() const final;

protected:
    void shoot();

private:
    int speed_ = -1;
};

class MediumTank : public Tank {
public:
    explicit MediumTank(int left,
                        int top,
                        int entityId,
                        const TankHandlerCreator &handlerCreator,
                        Direction direction,
                        int speed);

    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;

    [[nodiscard]] int getStrength() const override;
    [[nodiscard]] EntityType getType() const override;
};

}  // namespace Tanks::model

#endif  // TANKS_TANK_H
