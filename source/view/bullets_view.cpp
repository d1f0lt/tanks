#include "view/bullets_view.h"
#include <cassert>
#include "constants.h"

namespace Tanks::View {

BulletsSpriteHolder::BulletsSpriteHolder(const std::string &filename) {
    image.loadFromFile(filename);
    image.createMaskFromColor(sf::Color(0, 0, 0));
    texture.loadFromImage(image);
    sprite.setTexture(texture);
}

void BulletsSpriteHolder::draw(
    sf::RenderWindow &window,
    const std::vector<const model::Entity *> &bullets) {
    for (const auto *item : bullets) {
        assert(item->getType() == model::EntityType::BULLET);
        sf::Vector2<float> bulletPosition{
            static_cast<float>(item->getLeft() + MARGIN_LEFT),
            static_cast<float>(item->getTop() + MARGIN_TOP)};
        sprite.setPosition(bulletPosition);
        window.draw(sprite);
    }
}

}  // namespace Tanks::View