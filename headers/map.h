#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include "constants.h"

namespace Tanks {

enum class Blocks {LEFT_UP_CORNER, RIGHT_UP_CORNER, LEFT_DOWN_CORNER, RIGHT_DOWN_CORNER,
                   VERTICAL_BORDER, HORIZONTAL_BORDER,
                   FLOOR, BRICK, STEEL, WATER};

struct Map final {
public:
    explicit Map(const std::string &filename);

    void loadLevel(int level);

    void drawMap(sf::RenderWindow &window);

private:
    std::vector<std::vector<Blocks>> map;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
};

}

#endif // MAP_H