#ifndef BULLETS_VIEW_H
#define BULLETS_VIEW_H

#include <vector>
#include "model/entity.h"
#include <SFML/Graphics.hpp>

namespace Tanks::View {

struct BulletsSpriteHolder final {

    BulletsSpriteHolder(const std::string &filename);

    void draw(sf::RenderWindow &window, const std::vector<const model::Entity *> &bullets);

private:
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

};

}

#endif

