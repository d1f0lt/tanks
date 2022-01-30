#ifndef TANK_H_
#define TANK_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

namespace Tanks {

enum class Direction { LEFT, RIGHT, DOWN, UP };

struct Tank {
public:
    explicit Tank(const std::string &filename);

    virtual void update_position(Direction direction,
                                 double &current_frame,
                                 double time,
                                 const std::vector<int> &image_pos) = 0;

    [[nodiscard]] virtual const sf::Sprite &get_tank_sprite() const = 0;

protected:
    sf::Image tank_image;
    sf::Texture tank_texture;
    sf::Sprite tank_sprite;
    double speed = 0.05;
};

}  // namespace Tanks

#endif  // TANK_H_