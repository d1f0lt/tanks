#include "view/tank_view.h"
#include <cassert>

namespace Tanks::View {

TankSpriteHolder::TankSpriteHolder(const std::string &filename) {
    image.loadFromFile(filename);
    texture.loadFromImage(image);

    static const std::vector<sf::IntRect> rect = {
        sf::IntRect(24, 26, 24, 20), sf::IntRect(0, 26, 24, 20),
        sf::IntRect(0, 0, 20, 26), sf::IntRect(20, 0, 20, 26)};

    static const std::vector<sf::Vector2<float>> scale = {
        {2, 2.4}, {2, 2.4}, {2.4, 1.846}, {2.4, 1.846}};

    for (int i = 0; i < 4; i++) {
        sprites[i].setTexture(texture);
        sprites[i].setTextureRect(rect[i]);
        sprites[i].setScale(scale[i]);
    }
}

sf::Sprite &TankSpriteHolder::getCurrentSprite(const model::Tank &tank) {
    switch (tank.getDirection()) {
        case model::Direction::LEFT:
            return sprites[0];

        case model::Direction::RIGHT:
            return sprites[1];

        case model::Direction::UP:
            return sprites[2];

        case model::Direction::DOWN:
            return sprites[3];
    }
    assert(false);
}

void TankSpriteHolder::draw(sf::RenderWindow &window, const model::Tank &tank) {
    auto &sprite = getCurrentSprite(tank);
    sprite.setPosition(static_cast<float>(MARGIN_LEFT + tank.getLeft()),
                       MARGIN_TOP + static_cast<float>(tank.getTop()));
    window.draw(sprite);
}

}  // namespace Tanks::View