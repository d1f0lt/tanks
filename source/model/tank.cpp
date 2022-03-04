#include "tank.h"
#include "game_map.h"

namespace Tanks::model {
void PlayableTank::move(Direction dir) {
    setDirection(dir);
    move_(dir);
}

PlayableTank::PlayableTank(int left, int top, GameMap &map_)
    : Tank(left, top, EntityType::PLAYABLE_TANK, map_) {
}

Direction Tank::getDirection() const {
    return direction;
}

Tank::Tank(int left, int top, EntityType type_, GameMap &map_)
    : ForegroundEntity(left, top, TANK_SIZE, TANK_SIZE, type_, map_) {
}

void Tank::setDirection(Direction dir) {
    direction = dir;
}
}  // namespace Tanks::model
