#include "player.h"
#include <string>

namespace Tanks {

Player::Player(const std::string &filename, sf::Vector2<int> position)
    : Tank(filename, position) {
}

void Player::change_tail() {
    switch (get_direction()) {
        case Direction::LEFT:
            tank_sprite.setTextureRect(sf::IntRect(24, 26, 24, 20));
            tank_sprite.setScale(2, 2.4);  // 24, 20 -> 48, 48
            break;

        case Direction::RIGHT:
            tank_sprite.setTextureRect(sf::IntRect(0, 26, 24, 20));
            tank_sprite.setScale(2, 2.4);
            break;

        case Direction::UP:
            tank_sprite.setTextureRect(sf::IntRect(0, 0, 20, 26));
            tank_sprite.setScale(2.4, 1.846);  // 20, 26 -> 48, 47.996
            break;

        case Direction::DOWN:
            tank_sprite.setTextureRect(sf::IntRect(20, 0, 20, 26));
            tank_sprite.setScale(2.4, 1.846);
            break;
    }

    tank_sprite.setPosition(sf::Vector2<float>(coordinates));
}

}  // namespace Tanks