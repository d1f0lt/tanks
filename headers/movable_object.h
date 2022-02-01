#ifndef TANKS_MOVABLE_OBJECT_H
#define TANKS_MOVABLE_OBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include "constants.h"

namespace Tanks {

struct MovableObject {
    explicit MovableObject(Tanks::Direction direction,
                           const sf::Vector2<float> &start_coordinates,
                           double speed = 0.3);  // test speed, by default 0.15

    void update_position(Direction direction, double time);

    virtual void change_tail() = 0;

    void set_speed(double new_speed);

    [[nodiscard]] sf::Vector2<float> get_object_coordinate() const;

    [[nodiscard]] Direction get_direction() const;

protected:
    double speed;
    sf::Vector2<float> coordinates;

private:
    Direction direction;
};

}  // namespace Tanks

#endif  // TANKS_MOVABLE_OBJECT_H
