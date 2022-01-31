#include "../headers/player.h"
#include <iostream>

namespace Tanks {


Player::Player(const std::string &filename,
               sf::Vector2<float> position)
    : Tank(filename, position) {

}

void Player::update_position(Direction new_direction,
                             double time) {

    direction = new_direction;
    std::string str_direction;
    sf::Vector2<float> position_difference{0, 0};
    switch (direction) {
        case Direction::UP:
            position_difference.y = static_cast<float>(speed * -1 * time);
            str_direction = "up";
            break;

        case Direction::DOWN:
            position_difference.y = static_cast<float>(speed * time);
            str_direction = "down";
            break;

        case Direction::LEFT:
            position_difference.x = static_cast<float>(speed * -1 * time);
            str_direction = "left";
            break;

        case Direction::RIGHT:
            position_difference.x = static_cast<float>(speed * time);
            str_direction = "right";
            break;
    }

    tank_image.loadFromFile("../images/tanks/player_tank_" + str_direction + ".png");
    tank_image.createMaskFromColor(sf::Color(32, 200, 248)); // delete background
    tank_texture.loadFromImage(tank_image);
    tank_sprite.setTexture(tank_texture);

    coordinates.x += position_difference.x;
    coordinates.y += position_difference.y;

    tank_sprite.setPosition(coordinates);

}

[[nodiscard]] const sf::Sprite &Player::get_tank_sprite() const {
    return tank_sprite;
}

}  // namespace Tanks