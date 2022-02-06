#ifndef TANKS_MOVABLE_OBJECT_H
#define TANKS_MOVABLE_OBJECT_H

#include <SFML/System/Vector2.hpp>
#include "constants.h"

namespace Tanks {

struct MovableObject {
    explicit MovableObject(Tanks::Direction direction,
                           const sf::Vector2<int> &start_coordinates,
                           double speed = 0.3);  // test speed, by default 0.15

    void update_position(Direction direction, double time);

    void set_speed(double new_speed);

    [[nodiscard]] const sf::Vector2<int> &get_object_coordinate() const;

    [[nodiscard]] Direction get_direction() const;

protected:
    double speed;
    sf::Vector2<int> coordinates;

private:
    Direction direction;
};

}  // namespace Tanks

#endif  // TANKS_MOVABLE_OBJECT_H
