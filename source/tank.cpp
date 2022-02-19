#include "tank.h"

namespace Tanks {

namespace {
[[nodiscard]] bool checkIntersectionOfRectangles(
    const sf::Vector2<int> &firstLeftUpCorner,
    const sf::Vector2<int> &secondLeftUpCorner,
    int firstTileSize = TANK_SIZE,
    int secondTileSize = TILE_SIZE) {
    const sf::Vector2<int> firstRightDownCorner = {
        firstLeftUpCorner.x + firstTileSize,
        firstLeftUpCorner.y + firstTileSize};
    const sf::Vector2<int> secondRightDownCorner = {
        secondLeftUpCorner.x + secondTileSize,
        secondLeftUpCorner.y + secondTileSize};

    return firstLeftUpCorner.x < secondRightDownCorner.x &&
           firstRightDownCorner.x > secondLeftUpCorner.x &&
           firstLeftUpCorner.y < secondRightDownCorner.y &&
           firstRightDownCorner.y > secondLeftUpCorner.y;
}
}  // namespace

Tank::Tank(const sf::Vector2<int> &start_coordinates)
    : MovableObject(Direction::UP, start_coordinates) {
}

void Tank::checkCollisionWithMap(Map &map, double time) {
    const std::vector<std::vector<Block *>> field = map.getMap();
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (const auto &item : field[row]) {
            sf::Vector2<int> blockCoordinates = item->getCoordinates();
            if (checkIntersectionOfRectangles(coordinates, blockCoordinates) && item->canIntersectWithTank()) {
                switch (getDirection()) {
                    case Direction::LEFT:
                        updatePosition(Direction::RIGHT, time);
                        direction = Direction::LEFT;
                        break;

                    case Direction::RIGHT:
                        updatePosition(Direction::LEFT, time);
                        direction = Direction::RIGHT;
                        break;

                    case Direction::UP:
                        updatePosition(Direction::DOWN, time);
                        direction = Direction::UP;
                        break;

                    case Direction::DOWN:
                        updatePosition(Direction::UP, time);
                        direction = Direction::DOWN;
                        break;
                }
            }
        }
    }
}

bool Tank::isHaveShot() const {
    return have_bullet;
}

void Tank::makeShot() {
    have_bullet = false;
}

void Tank::recoverBullet() {
    have_bullet = true;
}

}  // namespace Tanks