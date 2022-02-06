#ifndef TANK_H_
#define TANK_H_

#include "bullet.h"
#include "constants.h"

namespace Tanks {

struct Tank : MovableObject {
public:
    explicit Tank(const sf::Vector2<int> &start_coordinates);

//    [[nodiscard]] const sf::Sprite &get_tank_sprite() const;

    void check_interaction_with_map();

    bool is_have_shot() const;

    void make_shot();

    void recover_bullet();

private:
    bool have_bullet = true;
};

}  // namespace Tanks

#endif  // TANK_H_