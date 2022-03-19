#ifndef TANK_H_
#define TANK_H_

#include "bullet.h"
#include "constants.h"
#include "map.h"

namespace Tanks {

struct Tank : MovableObject {
public:
    explicit Tank(const sf::Vector2<int> &start_coordinates);

    void checkCollisionWithMap(Map &map, double time) final;

    [[nodiscard]] bool isHaveShot() const;

    void makeShot();

    void recoverBullet();

private:
    bool have_bullet = true;
};

}  // namespace Tanks

#endif  // TANK_H_