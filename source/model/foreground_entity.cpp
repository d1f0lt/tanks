#include "foreground_entity.h"

namespace Tanks::model {
ForegroundEntity::ForegroundEntity(int left,
                                   int top,
                                   int width,
                                   int height,
                                   EntityType type_,
                                   GameMap &map_)
    : Entity(left, top, width, height, type_),
      map(map_),
      background(height, std::vector<Entity *>(width, nullptr)) {
    setBackground();
}

void ForegroundEntity::restoreBackground() {
    for (int y = 0; y < +getHeight(); y++) {
        for (int x = 0; x < +getWidth(); x++) {
            map.insert(*background[y][x]);
        }
    }
}

void ForegroundEntity::setBackground() {
    int y0 = getTop();
    int x0 = getLeft();
    for (int y = getTop(); y < getTop() + getHeight(); y++) {
        for (int x = getLeft(); x < getLeft() + getWidth(); x++) {
            background[y - y0][x - x0] = &map.getEntityByCoords(x, y);
        }
    }
    map.insert(*this);
}

void ForegroundEntity::move_(Tanks::model::Direction dir) {
    restoreBackground();
    switch (dir) {
        case Direction::UP:
            setTop(getTop() - 1);
            break;
        case Direction::LEFT:
            setLeft(getLeft() - 1);
            break;
        case Direction::DOWN:
            setTop(getTop() + 1);
            break;
        case Direction::RIGHT:
            setLeft(getLeft() + 1);
            break;
    }
    setBackground();
}

}  // namespace Tanks::model
