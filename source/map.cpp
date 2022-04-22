#include "map.h"
#include <cassert>
#include <fstream>

namespace Tanks {

Block::Block(const sf::Vector2<int> &coordinates_) : coordinates(coordinates_) {
}

// void Block::destroyBlock() {
//    assert(
//        type ==
//        model::EntityType::BRICK);  // TODO remake, if we want destroy not only bricks
//    type = model::EntityType::FLOOR;
//}

const sf::Vector2<int> &Block::getCoordinates() const {
    return coordinates;
}

bool Block::canIntersectWithTank() const {
    return true;
}

FloorBlock::FloorBlock(const sf::Vector2<int> &coordinates_)
    : Block(coordinates_) {
}

bool FloorBlock::canIntersectWithTank() const {
    return false;
}

BlockSpriteHolder::BlockSpriteHolder(const model::EntityType type,
                                     sf::Texture &texture,
                                     const sf::Vector2<int> &coordinates) {
    sprite.setTexture(texture);
    sprite.setPosition(static_cast<float>(coordinates.x),
                       static_cast<float>(coordinates.y));

    switch (type) {
        case model::EntityType::FLOOR:
            block = std::make_unique<FloorBlock>(coordinates);
            break;
        default:
            block = std::make_unique<Block>(coordinates);
            break;
    }

    changeSprite(type);
}

void BlockSpriteHolder::changeSprite(model::EntityType newType) {
    switch (newType) {
        case model::EntityType::FLOOR:
            sprite.setTextureRect(
                sf::IntRect(2 * TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE));
            break;
        case model::EntityType::BRICK:
            sprite.setTextureRect(
                sf::IntRect(TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE));
            break;

        case model::EntityType::STEEL:
            sprite.setTextureRect(
                sf::IntRect(TILE_SIZE, 2 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
            break;

        case model::EntityType::VERTICAL_BORDER:
            sprite.setTextureRect(
                sf::IntRect(0, TILE_SIZE, TILE_SIZE, TILE_SIZE));
            break;

        case model::EntityType::HORIZONTAL_BORDER:
            sprite.setTextureRect(
                sf::IntRect(TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
            break;

        case model::EntityType::WATER:
            sprite.setTextureRect(
                sf::IntRect(3 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
            break;

        case model::EntityType::LEFT_UP_CORNER:
            sprite.setTextureRect(sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));
            break;

        case model::EntityType::LEFT_DOWN_CORNER:
            sprite.setTextureRect(
                sf::IntRect(0, 2 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
            break;

        case model::EntityType::RIGHT_UP_CORNER:
            sprite.setTextureRect(
                sf::IntRect(2 * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
            break;

        case model::EntityType::RIGHT_DOWN_CORNER:
            sprite.setTextureRect(sf::IntRect(2 * TILE_SIZE, 2 * TILE_SIZE,
                                              TILE_SIZE, TILE_SIZE));
            break;
    }
}

const sf::Sprite &BlockSpriteHolder::getSprite() const {
    return sprite;
}

Map::Map(const std::string &filename) : map(MAP_HEIGHT) {
    image.loadFromFile(filename);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
}

void Map::loadLevel(int level) {
    const std::string currentLevel =
        "../levels/level" + std::to_string(level) + ".csv";

    static std::unordered_map<char, model::EntityType> translateCharToEnum = {
        {'=', model::EntityType::HORIZONTAL_BORDER},
        {'|', model::EntityType::VERTICAL_BORDER},
        {'1', model::EntityType::BRICK},
        {'0', model::EntityType::STEEL},
        {' ', model::EntityType::FLOOR},
        {'~', model::EntityType::WATER},
        {'[', model::EntityType::LEFT_UP_CORNER},
        {']', model::EntityType::RIGHT_UP_CORNER},
        {'{', model::EntityType::LEFT_DOWN_CORNER},
        {'}', model::EntityType::RIGHT_DOWN_CORNER}};

    std::fstream file(currentLevel);
    assert(file.is_open() && "Unable to open map texture file");
    std::string str;
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        std::getline(file, str);
        for (int col = 0; col < MAP_WIDTH * 2 - 1;
             col += 2) {  // skipping delimiter
            int realCol = col / 2;
            map[row].emplace_back(
                BlockSpriteHolder(translateCharToEnum[str[col]], texture,
                                  {MARGIN_LEFT + TILE_SIZE * realCol,
                                   MARGIN_TOP + TILE_SIZE * row}));
        }
    }
}

void Map::drawMap(sf::RenderWindow &window) {
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (const auto &tile : map[row]) {
            window.draw(tile.getSprite());
        }
    }
}

std::vector<std::vector<Block *>> Map::getMap() {
    std::vector<std::vector<Block *>> ans(MAP_HEIGHT,
                                          std::vector<Block *>(MAP_WIDTH));
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            ans[row][col] = map[row][col].block.get();
        }
    }
    return ans;
}

}  // namespace Tanks