//
// Created by ilma4 on 2/6/22.
//

#include "movable_object_view.h"

namespace Tanks {

// rect - rectangle of texture
SpriteHolder::SpriteHolder(const std::string &imageFilename,
                           const std::vector<sf::IntRect> &rect,
                           const std::vector<sf::Vector2<float>> &scale) {
    sf::Image image;
    image.loadFromFile(imageFilename);
    texture.loadFromImage(image);
    for (int i = 0; i < 4; i++) {
        sprites[i].setTexture(texture);
        sprites[i].setTextureRect(rect[i]);
        sprites[i].setScale(scale[i]);
    }
}

[[nodiscard]] const SpriteHolder &getTankSpriteHolder() {
    static const std::string &imageFilename =
        "../images/tanks/player_tanks.png";

    static const std::vector<sf::IntRect> rect = {
        sf::IntRect(24, 26, 24, 20), sf::IntRect(0, 26, 24, 20),
        sf::IntRect(0, 0, 20, 26), sf::IntRect(20, 0, 20, 26)};

    static const std::vector<sf::Vector2<float>> scale = {
        {2, 2.4}, {2, 2.4}, {2.4, 1.846}, {2.4, 1.846}};

    static const SpriteHolder spriteHolder(imageFilename, rect, scale);

    return spriteHolder;
}

[[nodiscard]] sf::Sprite MovableView::getSprite() const {
    auto sprite = spriteHolder.getSprite(object.getDirection());
    sprite.setPosition(sf::Vector2<float>(object.getCoordinates()));
    return sprite;
}

MovableView::MovableView(const MovableObject &object_,
                         const std::string &filename)
    : object(object_), spriteHolder(getTankSpriteHolder()) {
}

[[nodiscard]] sf::Sprite SpriteHolder::getSprite(Direction direction) const {
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
