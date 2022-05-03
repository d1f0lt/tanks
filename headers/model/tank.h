#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "game_map.h"
#include "movable_entity.h"

namespace Tanks::model {
class Tank : public MovableEntity {
public:
    explicit Tank(int left_,
                  int top_,
                  Direction direction_,
                  std::unique_ptr<BasicHandler> handler_);

    explicit Tank(int left,
                  int top,
                  int id,
                  std::unique_ptr<BasicHandler> handler,
                  Direction direction,
                  int speed);

    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;

    [[nodiscard]] int getStrength() const override;
    [[nodiscard]] bool canPass(const Entity &other) const override;

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
