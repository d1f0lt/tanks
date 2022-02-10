#ifndef TANK_H_
#define TANK_H_

#include <list>
#include "bullet.h"
#include "constants.h"
#include "map.h"

namespace Tanks {

struct Tank : MovableObject {
public:
    explicit Tank(const sf::Vector2<int> &start_coordinates);

    void checkCollisionWithMap(std::list<Block *> &blocks) final;

    bool is_have_shot() const;

    void make_shot();

    void recover_bullet();

private:
    bool have_bullet = true;
};

}  // namespace Tanks

#endif  // TANK_H_