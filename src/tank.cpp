#include "tank.h"
#include "game_map.h"

namespace tanks::model {
void Tank::move(tanks::model::Direction dir, const GameMap &map) {
    if (dir == Direction::UP) {
        sf::Rect<int> a = getRect();
    }
}
void Tank::updateBackground(const GameMap &map) {
}
}  // namespace tanks::model
