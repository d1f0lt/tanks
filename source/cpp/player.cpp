#include "../headers/player.h"

namespace Tanks {

Player::Player(const std::string &filename,
               sf::Vector2<float> position,
               sf::IntRect tile)
    : Tank(filename) {
    tank_sprite.setTextureRect(tile);
    tank_sprite.setPosition(position.x, position.y);
}

void Player::update_position(Direction direction,
                             double &current_frame,
                             double time,
                             const std::vector<int> &image_pos) {
    assert(image_pos.size() == 4);  // for sf::IntRect

    current_frame += 0.005 * time - static_cast<int>(current_frame > 3) * 3;
    sf::Vector2<float> position_difference{0, 0};
    switch (direction) {
        case Direction::UP:
            position_difference.y = static_cast<float>(speed * -1 * time);
            break;

        case Direction::DOWN:
            position_difference.y = static_cast<float>(speed * time);
            break;

        case Direction::LEFT:
            position_difference.x = static_cast<float>(speed * -1 * time);
            break;

        case Direction::RIGHT:
            position_difference.x = static_cast<float>(speed * time);
            break;
    }

    tank_sprite.setTextureRect(
        sf::IntRect(image_pos[0] * static_cast<int>(current_frame),
                    image_pos[1], image_pos[2], image_pos[3]));
    tank_sprite.move(position_difference);
}

[[nodiscard]] const sf::Sprite &Player::get_tank_sprite() const {
    return tank_sprite;
}

}  // namespace Tanks