//
// Created by ilma4 on 2/6/22.
//

#include "movable_object_view.h"

namespace Tanks {
void MovableView::updatePosition() {
    switch (object.get_direction()) {
        case Direction::LEFT:
            sprite.setTextureRect(sf::IntRect(24, 26, 24, 20));
            sprite.setScale(2, 2.4);  // 24, 20 -> 48, 48
            break;

        case Direction::RIGHT:
            sprite.setTextureRect(sf::IntRect(0, 26, 24, 20));
            sprite.setScale(2, 2.4);
            break;

        case Direction::UP:
            sprite.setTextureRect(sf::IntRect(0, 0, 20, 26));
            sprite.setScale(2.4, 1.846);  // 20, 26 -> 48, 47.996
            break;

        case Direction::DOWN:
            sprite.setTextureRect(sf::IntRect(20, 0, 20, 26));
            sprite.setScale(2.4, 1.846);
            break;
    }
    sprite.setPosition(sf::Vector2<float>(object.get_object_coordinate()));
}

MovableView::MovableView(const MovableObject &object_,
                         const std::string &filename)
    : object(object_) {
    image.loadFromFile(filename);
    image.createMaskFromColor(sf::Color(32, 200, 248));  // delete background

    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 20, 26));
    sprite.scale(2.4, 1.846);
    sprite.setPosition(sf::Vector2<float>(object.get_object_coordinate()));
}

const sf::Sprite &MovableView::getObjectSprite() const {
    return sprite;
}

}  // namespace Tanks
