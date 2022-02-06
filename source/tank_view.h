//
// Created by ilma4 on 2/6/22.
//

#ifndef TANKS_TANK_VIEW_H
#define TANKS_TANK_VIEW_H
#include <SFML/Graphics.hpp>
#include "bullet.h"
#include "constants.h"
#include "movable_object.h"
#include "tank.h"

namespace Tanks {
//struct Tank : MovableObject {
//public:
//    explicit Tank(const std::string &filename,
//                  const sf::Vector2<float> &start_coordinates);
//
//    [[nodiscard]] const sf::Sprite &get_tank_sprite() const;
//
//    void check_interaction_with_map();
//
//    bool is_have_shot() const;
//
//    void make_shot();
//
//    void recover_bullet();
//
//protected:
//    sf::Image tank_image;
//    sf::Texture tank_texture;
//    sf::Sprite tank_sprite;
//
//private:
//    bool have_bullet = true;
//};

struct Tank_view {
public:
    explicit Tank_view(const Tank &tank_, const std::string &filename)
        : tank(tank_) {
        tank_image.loadFromFile(filename);
        tank_image.createMaskFromColor(
            sf::Color(32, 200, 248));  // delete background
        tank_texture.loadFromImage(tank_image);
        tank_sprite.setTexture(tank_texture);
        tank_sprite.setTextureRect(sf::IntRect(0, 0, 20, 26));
        tank_sprite.scale(2.4, 1.846);
        //        tank_sprite.setPosition(tank_.coordinates);
    }

    //    Tank::Tank(const std::string &filename,
    //               const sf::Vector2<float> &start_coordinates)
    //        : MovableObject(Direction::UP, start_coordinates) {
    //        tank_image.loadFromFile(filename);
    //
    //    }

private:
    sf::Image tank_image;
    sf::Texture tank_texture;
    sf::Sprite tank_sprite;
    const Tank &tank;
};
}  // namespace Tanks

#endif  // TANKS_TANK_VIEW_H
