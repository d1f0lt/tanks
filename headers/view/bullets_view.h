#ifndef BULLETS_VIEW_H
#define BULLETS_VIEW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "model/entity.h"

namespace Tanks::View {

struct BulletsSpriteHolder final {
    BulletsSpriteHolder(const std::string &filename);

    void draw(sf::RenderWindow &window,
              const std::vector<const model::Entity *> &bullets);

private:
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
};

}  // namespace Tanks::View

#endif
