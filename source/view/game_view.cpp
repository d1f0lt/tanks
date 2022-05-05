#include "view/game_view.h"
#include <cassert>
#include <fstream>

namespace Tanks::View {

BlockSpriteHolder::BlockSpriteHolder(model::EntityType type_,
                                     sf::Texture &texture,
                                     const sf::Vector2<int> &coordinates)
    : type(type_) {
    sprite.setTexture(texture);
    sprite.setPosition(static_cast<float>(coordinates.x),
                       static_cast<float>(coordinates.y));
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
        default:
            assert(false && "unknown block type");
    }
    type = newType;
}

void BlockSpriteHolder::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
}

model::EntityType BlockSpriteHolder::getType() const {
    return type;
}

void BlockSpriteHolder::destroy() {
    changeSprite(model::EntityType::FLOOR);
}

Map::Map(const std::string &filename, int level) : map(MAP_HEIGHT) {
    image.loadFromFile(filename);
    texture.loadFromImage(image);
    loadLevel(level);
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
    assert(map[0].size() == MAP_WIDTH);
}

void Map::draw(sf::RenderWindow &window, model::GameModel &model) {
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            auto &tile = map[row][col];
            if (tile.getType() == model::EntityType::BRICK &&
                model.getEntityByCoords(col * TILE_SIZE, row * TILE_SIZE)
                        .getType() != model::EntityType::BRICK) {
                tile.destroy();
            }
            tile.draw(window);
        }
    }
}

}  // namespace Tanks::View