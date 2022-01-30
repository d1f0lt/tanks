#ifndef PLAYER_H_
#define PLAYER_H_

#include <cassert>
#include "tank.h"
#include "constants.h"

namespace Tanks {

struct Player final : public Tank {
public:
    explicit Player(const std::string &filename,
                    sf::Vector2<float> position,
                    sf::IntRect tile = {0, 0, Tanks::ONE_TILE_WIDTH, Tanks::ONE_TILE_HEIGHT});

    void update_position(Direction direction,
                         double &current_frame,
                         double time,
                         const std::vector<int> &image_pos) final;

    [[nodiscard]] const sf::Sprite &get_tank_sprite() const final;

private:

};

}  // namespace Tanks

#endif  // PLAYER_H_