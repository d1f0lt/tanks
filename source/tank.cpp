#include "tank.h"

namespace Tanks {

Tank::Tank(const sf::Vector2<int> &start_coordinates)
    : MovableObject(Direction::UP, start_coordinates) {
}

// TODO
// void Tank::check_interaction_with_map() {  // The coordinates were chosen
//                                           // empirically.
//    static const int shift = ONE_TILE_WIDTH * SHIFT;
//    coordinates.x = (coordinates.x < shift + 25 ? shift + 25 : coordinates.x);
//    coordinates.x = (coordinates.x > WINDOW_WIDTH - 32 - 45 - shift
//                         ? WINDOW_WIDTH - 77 - shift
//                         : coordinates.x);
//    coordinates.y = (coordinates.y < 25 ? 25 : coordinates.y);
//    coordinates.y =
//        (coordinates.y > WINDOW_HEIGHT - 32 - 45 ? WINDOW_HEIGHT - 77
//                                                 : coordinates.y);
//}

bool Tank::is_have_shot() const {
    return have_bullet;
}

void Tank::make_shot() {
    have_bullet = false;
}

void Tank::recover_bullet() {
    have_bullet = true;
}

}  // namespace Tanks