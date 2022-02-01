#include "../headers/movable_object.h"

namespace Tanks {

MovableObject::MovableObject(Tanks::Direction direction,
                             const sf::Vector2<float> &start_coordinates,
                             double speed)
    : direction(direction), coordinates(start_coordinates), speed(speed) {
}

void MovableObject::update_position(Direction new_direction, double time) {
    direction = new_direction;
    switch (direction) {
        case Direction::UP:
            coordinates.y += static_cast<float>(speed * -1 * time);
            break;

        case Direction::DOWN:
            coordinates.y += static_cast<float>(speed * time);
            break;

        case Direction::LEFT:
            coordinates.x += static_cast<float>(speed * -1 * time);
            break;

        case Direction::RIGHT:
            coordinates.x += static_cast<float>(speed * time);
            break;
    }

    change_tail();
}

void MovableObject::set_speed(double new_speed) {
    speed = new_speed;
}

[[nodiscard]] sf::Vector2<float> MovableObject::get_object_coordinate() const {
    return coordinates;
}

[[nodiscard]] Direction MovableObject::get_direction() const {
    return direction;
}

}  // namespace Tanks
