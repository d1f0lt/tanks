#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <memory>
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
    explicit Block(const sf::Vector2<int> &coordinates_);

    [[nodiscard]] const sf::Vector2<int> &getCoordinates() const;

    [[nodiscard]] virtual bool canIntersectWithTank() const;  // TODO rename

    //    void destroyBlock();

private:
    const sf::Vector2<int> coordinates;
};

struct FloorBlock final : Block {  // TODO add new
public:
    explicit FloorBlock(const sf::Vector2<int> &coordinates_);

    [[nodiscard]] bool canIntersectWithTank() const final;
};

struct BlockSpriteHolder {
public:
    BlockSpriteHolder(BlockType type,
                      sf::Texture &texture,
                      const sf::Vector2<int> &coordinates_);

    const sf::Sprite &getSprite() const;

private:
    std::unique_ptr<Block> block;
    sf::Sprite sprite;

    void changeSprite(BlockType new_type);

    friend struct Map;
};

struct Map final {
public:
    explicit Map(const std::string &filename);

    void loadLevel(int level);

    void drawMap(sf::RenderWindow &window);

private:
    std::vector<std::vector<BlockSpriteHolder>> map;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    std::vector<std::vector<Block *>> getMap();

    friend struct Tank;
};

}  // namespace Tanks

#endif  // MAP_H