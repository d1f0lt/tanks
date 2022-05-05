#ifndef TANK_H
#define TANK_H

#include <string>
#include "SFML/Graphics.hpp"
#include "model/tank.h"

namespace Tanks::View {

struct TankSpriteHolder final {
public:
    TankSpriteHolder(const std::string &filename);

    void draw(sf::RenderWindow &window, const model::Tank &tank);

    sf::Sprite &getCurrentSprite(const model::Tank &tank);

private:
    sf::Image image;
    sf::Texture texture;
    std::vector<sf::Sprite> sprites{4};
};
}  // namespace Tanks::View

#endif