#include "bullet.h"

namespace Tanks {

Bullet::Bullet(Direction direction, const sf::Vector2<float> &start_coordinates)
    : MovableObject(direction, start_coordinates, 0.4) {
    bullet_image.loadFromFile("../images/bullet.png");
    bullet_image.createMaskFromColor(sf::Color(0, 0, 0));
    bullet_texture.loadFromImage(bullet_image);
    bullet_sprite.setTexture(bullet_texture);
//    coordinates.x += 18; TODO
//    coordinates.y += 14;
    bullet_sprite.setPosition(coordinates);
}

void Bullet::change_tail() {
    bullet_sprite.setTexture(bullet_texture);
    bullet_sprite.setPosition(coordinates);
}

bool Bullet::is_destroyed() const {
    return destroyed;
}

const sf::Sprite &Bullet::get_bullet_sprite() const {
    return bullet_sprite;
}

// TODO
//void Bullet::check_intersection_with_map() {
//    if (coordinates.x < SHIFT * ONE_TILE_WIDTH + 20 ||
//        coordinates.x > WINDOW_WIDTH - (SHIFT + 1) * ONE_TILE_WIDTH - 5 ||
//        coordinates.y > WINDOW_HEIGHT - 45 || coordinates.y < 32) {
//        destroyed = true;
//    }
//}

}  // namespace Tanks