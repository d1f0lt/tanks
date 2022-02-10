#ifndef TANKS_MOVABLE_OBJECT_H
#define TANKS_MOVABLE_OBJECT_H

#include <SFML/System/Vector2.hpp>
#include <list>
#include "constants.h"
#include "map.h"

namespace Tanks {

struct MovableObject {
    explicit MovableObject(Direction direction,
                           const sf::Vector2<int> &startCoordinates,
                           double speed = 0.3);  // test speed, by default 0.15

    void updatePosition(Direction newDirection, double time);

    virtual void checkCollisionWithMap(std::list<Block *> &blocks) = 0;

    void setSpeed(double new_speed);

    [[nodiscard]] const sf::Vector2<int> &getCoordinates() const;

    [[nodiscard]] Direction getDirection() const;

protected:
    double speed;
    sf::Vector2<int> coordinates;

private:
    Direction direction;
};

}  // namespace Tanks

#endif  // TANKS_MOVABLE_OBJECT_H
