#include "../headers/tank.h"

namespace Tanks {

Tank::Tank(const std::string &filename) {
    tank_image.loadFromFile(filename);
    tank_texture.loadFromImage(tank_image);
    tank_sprite.setTexture(tank_texture);
    tank_sprite.scale({0.5, 0.5});
}

}  // namespace Tanks