#include "../headers/tank.h"

namespace Tanks {

Tank::Tank(const std::string &filename, const sf::Vector2<float> &start_coordinates) : coordinates(start_coordinates) {
    tank_image.loadFromFile(filename);
    tank_image.createMaskFromColor(sf::Color(32, 200, 248)); // delete background
    tank_texture.loadFromImage(tank_image);
    tank_sprite.setTexture(tank_texture);
    tank_sprite.scale(1.6, 1.6);
    tank_sprite.setPosition(coordinates);
    direction = Direction::DOWN;
}

float Tank::get_tank_coordinate_x() const {
    return coordinates.x;
}

float Tank::get_tank_coordinate_y() const {
    return coordinates.y;
}

void Tank::interaction_with_map() { // The coordinates were chosen empirically.
    coordinates.x = (coordinates.x < 25 ? 25 : coordinates.x);
    coordinates.x = (coordinates.x > WINDOW_WIDTH - 32 - 45 ? WINDOW_WIDTH - 77 : coordinates.x);
    coordinates.y = (coordinates.y < 25 ? 25 : coordinates.y);
    coordinates.y = (coordinates.y > WINDOW_HEIGHT - 32 - 45 ? WINDOW_HEIGHT - 77 : coordinates.y);
}

}  // namespace Tanks