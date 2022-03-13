#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "foreground_entity.h"
#include "game_map.h"

namespace Tanks::model {
class Tank : public ForegroundEntity {
public:
    Tank(int left, int top, EntityType type_, GameMap &map_);

    [[nodiscard]] Direction getDirection() const;

    [[nodiscard]] std::vector<const Entity *> whatsOn(Direction dir);

    void shoot();

protected:
    void setDirection(Direction dir);

private:
    Direction direction = Direction::DOWN;
};

class PlayableTank : public Tank {
public:
    PlayableTank(int left, int top, GameMap &map_);
    void move(Direction dir);
};
}  // namespace Tanks::model

#endif  // TANKS_TANK_H
