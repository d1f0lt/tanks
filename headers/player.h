#ifndef PLAYER_H_
#define PLAYER_H_

#include <cassert>
#include "constants.h"
#include "tank.h"

namespace Tanks {

struct Player final : public Tank {
public:
    explicit Player(const std::string &filename,
                    sf::Vector2<float> position);

    void update_position(Direction direction,
                         double time) final;

    [[nodiscard]] const sf::Sprite &get_tank_sprite() const final;

private:
};

}  // namespace Tanks

#endif  // PLAYER_H_