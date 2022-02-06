//
// Created by ilma4 on 2/6/22.
//

#ifndef TANKS_MOVABLE_OBJECT_VIEW_H
#define TANKS_MOVABLE_OBJECT_VIEW_H

#include <SFML/Graphics.hpp>
#include <cassert>
#include <utility>
#include "constants.h"
#include "movable_object.h"

namespace Tanks {
// TODO refactor it
struct SpriteHolder {
    [[nodiscard]] sf::Sprite getSprite(Direction direction) const {
        switch (direction) {
            case Direction::LEFT:
                return sprites[0];

            case Direction::RIGHT:
                return sprites[1];

            case Direction::UP:
                return sprites[2];

            case Direction::DOWN:
                return sprites[3];
        }
        assert("Incorrect direction? 0_o" == "");
    }

    // Contructor for tank
    explicit SpriteHolder(const std::string &filename) {
        const sf::IntRect rect[] = {
            sf::IntRect(24, 26, 24, 20), sf::IntRect(0, 26, 24, 20),
            sf::IntRect(0, 0, 20, 26), sf::IntRect(20, 0, 20, 26)};

        const sf::Vector2<float> scale[4] = {
            {2, 2.4}, {2, 2.4}, {2.4, 1.846}, {2.4, 1.846}};

        textures.resize(4);
        sprites.resize(4);

        for (int i = 0; i < 4; i++) {
            sf::Image image;
            image.loadFromFile(filename);
            image.createMaskFromColor(
                sf::Color(32, 200, 248));  // delete background
            textures[i].loadFromImage(image);
            sprites[i].setTexture(textures[i]);
            sprites[i].setTextureRect(rect[i]);
            sprites[i].setScale(scale[i]);
        }
    }

    explicit SpriteHolder(const std::string &imageFilename,
                          const std::vector<sf::IntRect> &rect,
                          const std::vector<sf::Vector2<float>> &scale,
                          const sf::Color &color) {
        textures.resize(4);
        sprites.resize(4);

        for (int i = 0; i < 4; i++) {
            sf::Image image;
            image.loadFromFile(imageFilename);
            image.createMaskFromColor(color);
            textures[i].loadFromImage(image);
            sprites[i].setTexture(textures[i]);
            sprites[i].setTextureRect(rect[i]);
            sprites[i].setScale(scale[i]);
        }
    }

private:
    std::vector<sf::Texture> textures;
    std::vector<sf::Sprite> sprites;
};

struct MovableView {
public:
    explicit MovableView(const MovableObject &object_,
                         const std::string &filename);

    [[nodiscard]] sf::Sprite getSprite() const;

private:
    const SpriteHolder holder;  // TODO make it link
    const MovableObject &object;
};

}  // namespace Tanks

#endif  // TANKS_MOVABLE_OBJECT_VIEW_H
