#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <list>
#include "constants.h"

namespace Tanks {

enum class BlockType {
    LEFT_UP_CORNER,
    RIGHT_UP_CORNER,
    LEFT_DOWN_CORNER,
    RIGHT_DOWN_CORNER,
    VERTICAL_BORDER,
    HORIZONTAL_BORDER,
    FLOOR,
    BRICK,
    STEEL,
    WATER
};

struct Block {
public:
    explicit Block(const sf::Vector2<int> &coordinates_, BlockType type_);

    [[nodiscard]] const sf::Vector2<int> &getCoordinates() const;

    void destroyBlock();

private:
    const sf::Vector2<int> coordinates;
    BlockType type;

    friend struct Map;
};

struct Map final {
public:
    explicit Map(const std::string &filename);

    void loadLevel(int level);

    void drawMap(sf::RenderWindow &window);

    std::list<Block *> getPhysicalMapBlocks();

private:
    std::vector<std::vector<Block>> map;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
};

}  // namespace Tanks

#endif  // MAP_H