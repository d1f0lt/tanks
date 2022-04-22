#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "constants.h"

namespace Tanks {

struct Block {
public:
    explicit Block(const sf::Vector2<int> &coordinates_);

    [[nodiscard]] const sf::Vector2<int> &getCoordinates() const;

    [[nodiscard]] virtual bool canIntersectWithTank() const;  // TODO rename


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
    BlockSpriteHolder(model::EntityType type,
                      sf::Texture &texture,
                      const sf::Vector2<int> &coordinates_);

    const sf::Sprite &getSprite() const;

private:
    sf::Sprite sprite;

    void changeSprite(model::EntityType new_type);

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