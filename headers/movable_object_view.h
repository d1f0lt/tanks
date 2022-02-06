//
// Created by ilma4 on 2/6/22.
//

#ifndef TANKS_MOVABLE_OBJECT_VIEW_H
#define TANKS_MOVABLE_OBJECT_VIEW_H
#include <SFML/Graphics.hpp>
#include "bullet.h"
#include "constants.h"
#include "movable_object.h"
#include "tank.h"

namespace Tanks {
struct MovableView {
public:
    explicit MovableView(const MovableObject &object_,
                         const std::string &filename);

    [[nodiscard]] const sf::Sprite &getObjectSprite() const;
    void updatePosition();

private:
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    const MovableObject &object;
};

}  // namespace Tanks

#endif  // TANKS_MOVABLE_OBJECT_VIEW_H
