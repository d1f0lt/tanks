#include "map.h"
#include <cassert>
#include <fstream>

namespace Tanks {

Block::Block(const sf::Vector2<int> &coordinates_,
             const sf::Vector2<int> &mapPlacement_,
             BlockType type_)
    : coordinates(coordinates_), mapPlacement(mapPlacement_), type(type_) {
}

const sf::Vector2<int> &Block::getCoordinates() const {
    return coordinates;
}

Map::Map(const std::string &filename) : map(MAP_HEIGHT) {
    image.loadFromFile(filename);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
}

void Map::loadLevel(int level) {
    const std::string currentLevel =
        "../levels/level" + std::to_string(level) + ".csv";

    static std::unordered_map<char, BlockType> translateStringToEnum = {
        {'=', BlockType::HORIZONTAL_BORDER},
        {'|', BlockType::VERTICAL_BORDER},
        {'1', BlockType::BRICK},
        {'0', BlockType::STEEL},
        {' ', BlockType::FLOOR},
        {'~', BlockType::WATER},
        {'[', BlockType::LEFT_UP_CORNER},
        {']', BlockType::RIGHT_UP_CORNER},
        {'{', BlockType::LEFT_DOWN_CORNER},
        {'}', BlockType::RIGHT_DOWN_CORNER}};

    std::fstream file(currentLevel);
    assert(file.is_open());
    std::string str;
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        std::getline(file, str);
        for (int col = 0; col < MAP_WIDTH * 2 - 1;
             col += 2) {  // skipping delimiter
            int realCol = col / 2;
            map[row].emplace_back(Block({MARGIN_LEFT + TILE_SIZE * realCol,
                                         MARGIN_TOP + TILE_SIZE * row},
                                        {row, realCol},
                                        translateStringToEnum[str[col]]));
        }
    }
}

void Map::drawMap(sf::RenderWindow &window) {
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            BlockType item = map[row][col].type;
            switch (item) {
                case BlockType::FLOOR:
                    sprite.setTextureRect(sf::IntRect(2 * TILE_SIZE, TILE_SIZE,
                                                      TILE_SIZE, TILE_SIZE));
                    break;
                case BlockType::BRICK:
                    sprite.setTextureRect(sf::IntRect(TILE_SIZE, TILE_SIZE,
                                                      TILE_SIZE, TILE_SIZE));
                    break;

                case BlockType::STEEL:
                    sprite.setTextureRect(sf::IntRect(TILE_SIZE, 2 * TILE_SIZE,
                                                      TILE_SIZE, TILE_SIZE));
                    break;

                case BlockType::VERTICAL_BORDER:
                    sprite.setTextureRect(
                        sf::IntRect(0, TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    break;

                case BlockType::HORIZONTAL_BORDER:
                    sprite.setTextureRect(
                        sf::IntRect(TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
                    break;

                case BlockType::WATER:
                    sprite.setTextureRect(
                        sf::IntRect(3 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
                    break;

                case BlockType::LEFT_UP_CORNER:
                    sprite.setTextureRect(
                        sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));
                    break;

                case BlockType::LEFT_DOWN_CORNER:
                    sprite.setTextureRect(
                        sf::IntRect(0, 2 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    break;

                case BlockType::RIGHT_UP_CORNER:
                    sprite.setTextureRect(
                        sf::IntRect(2 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
                    break;

                case BlockType::RIGHT_DOWN_CORNER:
                    sprite.setTextureRect(sf::IntRect(
                        2 * TILE_SIZE, 2 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    break;
            }
            sprite.setPosition(
                static_cast<sf::Vector2<float>>(map[row][col].coordinates));

            window.draw(sprite);
        }
    }
}

std::list<Block *> Map::getPhysicalMapBlocks() {
    std::list<Block *> ans;
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            if (map[row][col].type != BlockType::FLOOR) {
                ans.emplace_back(&map[row][col]);
            }
        }
    }
    return ans;
}

void Map::destroyBlock(int row, int col) {
    assert(map[row][col].type != BlockType::FLOOR);
    map[row][col].type = BlockType::FLOOR;
}

}  // namespace Tanks