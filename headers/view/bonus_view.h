#ifndef BONUS_VIEW_H
#define BONUS_VIEW_H

#include <SFML/Graphics.hpp>
#include "model/bonus.h"

namespace Tanks::View {

struct BonusView {
public:
    BonusView(const std::string &filename);

    void draw(sf::RenderWindow &window, const Tanks::model::WalkOnWater *bonus);

private:
    sf::Image image;
    sf::Texture texture;
    mutable sf::Sprite sprite;
};

}

#endif