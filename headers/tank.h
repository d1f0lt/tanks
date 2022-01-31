#ifndef TANK_H_
#define TANK_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "constants.h"

namespace Tanks {

struct Tank {
public:
    explicit Tank(const std::string &filename, const sf::Vector2<float> &start_coordinates);

    virtual void update_position(Direction direction, double time) = 0;

    [[nodiscard]] virtual const sf::Sprite &get_tank_sprite() const = 0;

    [[nodiscard]] float get_tank_coordinate_x() const;

    [[nodiscard]] float get_tank_coordinate_y() const;

    void interaction_with_map();

protected:
    sf::Image tank_image;
    sf::Texture tank_texture;
    sf::Sprite tank_sprite;
    sf::Vector2<float> coordinates;
    Tanks::Direction direction;
    double speed = 0.3;
};

}  // namespace Tanks

#endif  // TANK_H_