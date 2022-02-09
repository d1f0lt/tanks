#include "map.h"
#include <fstream>
#include <cassert>

namespace Tanks {

Map::Map(const std::string &filename) : map(MAP_HEIGHT, std::vector<Blocks>(MAP_WIDTH, Blocks::FLOOR)) {
    image.loadFromFile(filename);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
}

void Map::loadLevel(int level) {
    const std::string currentLevel =  "../levels/level" + std::to_string(level) + ".csv";


    static std::unordered_map<char, Blocks> translateStringToEnum = {
            {'=', Blocks::HORIZONTAL_BORDER}, {'|', Blocks::VERTICAL_BORDER},
            {'1', Blocks::BRICK}, {'0', Blocks::STEEL},
            {' ', Blocks::FLOOR}, {'~', Blocks::WATER},
            {'[', Blocks::LEFT_UP_CORNER}, {']', Blocks::RIGHT_UP_CORNER},
            {'{', Blocks::LEFT_DOWN_CORNER}, {'}', Blocks::RIGHT_DOWN_CORNER}
    };


    std::fstream file(currentLevel);
    assert(file.is_open());
    std::string str;
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        std::getline(file, str);
        for (int j = 0; j < MAP_WIDTH * 2 - 1; j += 2) { // skipping delimiter
            map[i][j/2] = translateStringToEnum[str[j]];
        }
    }
}

void Map::drawMap(sf::RenderWindow &window) {
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            Blocks item = map[row][col];
            switch (item) {
                case Blocks::FLOOR:
                    sprite.setTextureRect(sf::IntRect(2 * TILE_SIZE, TILE_SIZE,
                                        TILE_SIZE, TILE_SIZE));
                    break;
                case Blocks::BRICK:
                    sprite.setTextureRect(sf::IntRect(TILE_SIZE, TILE_SIZE,
                                        TILE_SIZE, TILE_SIZE));
                    break;

                case Blocks::STEEL:
                    sprite.setTextureRect(sf::IntRect(TILE_SIZE, 2 * TILE_SIZE,
                                        TILE_SIZE, TILE_SIZE));
                    break;

                case Blocks::VERTICAL_BORDER:
                    sprite.setTextureRect(sf::IntRect(0, TILE_SIZE,
                                                      TILE_SIZE, TILE_SIZE));
                    break;

                case Blocks::HORIZONTAL_BORDER:
                    sprite.setTextureRect(sf::IntRect(
                            TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
                    break;

                case Blocks::WATER:
                    sprite.setTextureRect(sf::IntRect(3 * TILE_SIZE, 0,
                                                      TILE_SIZE,
                                                      TILE_SIZE));
                    break;

                case Blocks::LEFT_UP_CORNER:
                    sprite.setTextureRect(
                            sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));
                    break;

                case Blocks::LEFT_DOWN_CORNER:
                    sprite.setTextureRect(sf::IntRect(0, 2 * TILE_SIZE,
                                                      TILE_SIZE,
                                                      TILE_SIZE));
                    break;

                case Blocks::RIGHT_UP_CORNER:
                    sprite.setTextureRect(sf::IntRect(2 * TILE_SIZE, 0,
                                                      TILE_SIZE,
                                                      TILE_SIZE));
                    break;

                case Blocks::RIGHT_DOWN_CORNER:
                    sprite.setTextureRect(
                            sf::IntRect(2 * TILE_SIZE, 2 * TILE_SIZE,
                                        TILE_SIZE, TILE_SIZE));
                    break;
            }
            sprite.setPosition(
                    static_cast<float>(MARGIN_LEFT + TILE_SIZE * col),
                    static_cast<float>(MARGIN_TOP + TILE_SIZE * row));

            window.draw(sprite);
        }
    }
}

}