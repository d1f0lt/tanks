//
// Created by ilma4 on 2/6/22.
//

#include "movable_object_view.h"

namespace Tanks {
// Contructor for Tank
SpriteHolder::SpriteHolder(const std::string &filename) {
    const sf::IntRect rect[] = {
        sf::IntRect(24, 26, 24, 20), sf::IntRect(0, 26, 24, 20),
        sf::IntRect(0, 0, 20, 26), sf::IntRect(20, 0, 20, 26)};

    const sf::Vector2<float> scale[4] = {
        {2, 2.4}, {2, 2.4}, {2.4, 1.846}, {2.4, 1.846}};
    sf::Image image;
    image.loadFromFile(filename);
    image.createMaskFromColor(sf::Color(32, 200, 248));  // delete background
    texture.loadFromImage(image);

    for (int i = 0; i < 4; i++) {
        sprites[i].setTexture(texture);
        sprites[i].setTextureRect(rect[i]);
        sprites[i].setScale(scale[i]);
    }
}

// rect - rectangle of texture
SpriteHolder::SpriteHolder(const std::string &imageFilename,
                           const std::vector<sf::IntRect> &rect,
                           const std::vector<sf::Vector2<float>> &scale,
                           const sf::Color &color) {
    sf::Image image;
    image.loadFromFile(imageFilename);
    image.createMaskFromColor(color);
    texture.loadFromImage(image);
    for (int i = 0; i < 4; i++) {
        sprites[i].setTexture(texture);
        sprites[i].setTextureRect(rect[i]);
        sprites[i].setScale(scale[i]);
    }
}

sf::Sprite MovableView::getSprite() const {
    auto sprite = holder.getSprite(object.get_direction());
    sprite.setPosition(sf::Vector2<float>(object.get_coordinates()));
    return sprite;
}

MovableView::MovableView(const MovableObject &object_,
                         const std::string &filename)
    : object(object_), holder(filename) {  // TODO : create one holder for tank
                                           // and use link to them in all tanks
}

sf::Sprite SpriteHolder::getSprite(Direction direction) const {
    switch (direction) {
        case Direction::LEFT:
            return sprites[0];

        case Direction::RIGHT:
            return sprites[1];

        case Direction::UP:
            return sprites[2];

        case Direction::DOWN:
            return sprites[3];
    }
    assert("Incorrect direction? 0_o" == "");
}

}  // namespace Tanks
