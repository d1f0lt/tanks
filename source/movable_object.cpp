#include "movable_object.h"

namespace Tanks {

MovableObject::MovableObject(Tanks::Direction direction,
                             const sf::Vector2<int> &startCoordinates,
                             double speed)
    : direction(direction), coordinates(startCoordinates), speed(speed) {
}

void MovableObject::updatePosition(Direction newDirection, double time) {
    direction = newDirection;
    switch (direction) {
        case Direction::UP:
            coordinates.y += static_cast<int>(speed * -1 * time);
            break;

        case Direction::DOWN:
            coordinates.y += static_cast<int>(speed * time);
            break;

        case Direction::LEFT:
            coordinates.x += static_cast<int>(speed * -1 * time);
            break;

        case Direction::RIGHT:
            coordinates.x += static_cast<int>(speed * time);
            break;
    }
}

void MovableObject::setSpeed(double new_speed) {
    speed = new_speed;
}

[[nodiscard]] const sf::Vector2<int> &MovableObject::getCoordinates() const {
    return coordinates;
}

[[nodiscard]] Direction MovableObject::getDirection() const {
    return direction;
}

}  // namespace Tanks
