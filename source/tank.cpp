#include "tank.h"

namespace Tanks {

namespace {
bool checkIntersectionOfRectangles(const sf::Vector2<int> &firstLeftUpCorner,
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

void Tank::checkCollisionWithMap(std::list<Block *> &blocks) {
    for (auto &item : blocks) {
        sf::Vector2<int> blockCoordinates = item->getCoordinates();
        if (checkIntersectionOfRectangles(coordinates, blockCoordinates)) {
            switch (getDirection()) {
                case Direction::LEFT:
                    coordinates.x = blockCoordinates.x + TILE_SIZE;
                    break;

                case Direction::RIGHT:
                    coordinates.x = blockCoordinates.x - TANK_SIZE;
                    break;

                case Direction::UP:
                    coordinates.y = blockCoordinates.y + TILE_SIZE;
                    break;

                case Direction::DOWN:
                    coordinates.y = blockCoordinates.y - TANK_SIZE;
                    break;
            }
        }
    }
}

bool Tank::is_have_shot() const {
    return have_bullet;
}

void Tank::make_shot() {
    have_bullet = false;
}

void Tank::recover_bullet() {
    have_bullet = true;
}

}  // namespace Tanks