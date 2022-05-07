#include "bullet.h"

namespace Tanks {

Bullet::Bullet(Direction direction, const sf::Vector2<int> &startCoordinates)
    : MovableObject(direction, startCoordinates, 0.4) {
    // TODO make MovableView for bullet
    //    bullet_image.loadFromFile("../images/bullet.png");
    //    bullet_image.createMaskFromColor(sf::Color(0, 0, 0));
    //    bullet_texture.loadFromImage(bullet_image);
    //    bullet_sprite.setTexture(bullet_texture);
    //    coordinates.x += 18; TODO
    //    coordinates.y += 14;
    //    bullet_sprite.setPosition(sf::Vector2<float>(coordinates));
}

bool Bullet::isDestroyed() const {
    return destroyed;
}

// TODO
// void Bullet::checkIntersectionWithMap() {
//    if (coordinates.x < SHIFT * ONE_TILE_WIDTH + 20 ||
//        coordinates.x > WINDOW_WIDTH - (SHIFT + 1) * ONE_TILE_WIDTH - 5 ||
//        coordinates.y > WINDOW_HEIGHT - 45 || coordinates.y < 32) {
//        destroyed = true;
//    }
//}

}  // namespace Tanks