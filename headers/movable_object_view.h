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
struct SpriteHolder {
    explicit SpriteHolder(const std::string &imageFilename,
                          const std::vector<sf::IntRect> &rect,
                          const std::vector<sf::Vector2<float>> &scale,
                          const sf::Color &color);

    [[nodiscard]] sf::Sprite getSprite(Direction direction) const;

private:
    sf::Texture texture;
    std::vector<sf::Sprite> sprites{4};
};

struct MovableView {
public:
    explicit MovableView(const MovableObject &object_,
                         const std::string &filename);

    [[nodiscard]] sf::Sprite getSprite() const;

private:
    const SpriteHolder &spriteHolder;
    const MovableObject &object;
};

}  // namespace Tanks

#endif  // TANKS_MOVABLE_OBJECT_VIEW_H
