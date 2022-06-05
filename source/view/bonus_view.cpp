#include "view/bonus_view.h"
#include <cassert>

namespace Tanks::View {

BonusView::BonusView(const std::string &filename) {
    image.loadFromFile(filename);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
}

void BonusView::draw(sf::RenderWindow &window,
                     const Tanks::model::WalkOnWater *bonus) {
    assert(bonus != nullptr);
    sprite.setPosition(static_cast<float>(bonus->getLeft() + Tanks::MARGIN_LEFT),  static_cast<float>(bonus->getTop() + Tanks::MARGIN_TOP));
    window.draw(sprite);
}

}