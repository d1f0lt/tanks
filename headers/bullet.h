#ifndef TANKS_BULLET_H
#define TANKS_BULLET_H

#include "constants.h"
#include "movable_object.h"

namespace Tanks {

struct Bullet final : MovableObject {
public:
    Bullet(Direction direction, const sf::Vector2<int> &startCoordinates);

    bool isDestroyed() const;

    void checkIntersectionWithMap();

private:
    bool destroyed = false;
};

}  // namespace Tanks

#endif  // TANKS_BULLET_H
