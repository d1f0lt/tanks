#include "../headers/player.h"
#include <string>
#include <unordered_map>

namespace Tanks {

Player::Player(const std::string &filename, sf::Vector2<float> position)
    : Tank(filename, position) {
}

void Player::change_tail() {
    static std::unordered_map<Direction, std::string>
        translate_direction_to_string{{Direction::UP, "up"},
                                      {Direction::DOWN, "down"},
                                      {Direction::LEFT, "left"},
                                      {Direction::RIGHT, "right"}};

    std::string str_direction(translate_direction_to_string[get_direction()]);

    tank_image.loadFromFile("../images/tanks/player_tank_" + str_direction +
                            ".png");
    tank_image.createMaskFromColor(
        sf::Color(32, 200, 248));  // delete background
    tank_texture.loadFromImage(tank_image);
    tank_sprite.setTexture(tank_texture);

    tank_sprite.setPosition(coordinates);
}

}  // namespace Tanks