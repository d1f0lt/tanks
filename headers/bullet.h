#ifndef TANKS_BULLET_H
#define TANKS_BULLET_H

#include "constants.h"
#include "movable_object.h"

namespace Tanks {

struct Bullet final : MovableObject {
public:
    Bullet(Direction direction, const sf::Vector2<int> &start_coordinates);

    //    void change_tail() final;

    bool is_destroyed() const;

    //    const sf::Sprite &get_bullet_sprite() const;

    void check_intersection_with_map();

private:
    bool destroyed = false;
};

}  // namespace Tanks

#endif  // TANKS_BULLET_H
