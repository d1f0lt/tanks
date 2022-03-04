#include "foreground_entity.h"

namespace Tanks::model {
void ForegroundEntity::restoreBackground() {
    for (int y = getTop(); y < getTop() + getHeight(); y++) {
        for (int x = getLeft(); x < getLeft() + getWidth(); x++) {
            map.insert(*background[y][x]);
        }
    }
}

void ForegroundEntity::setBackground() {
    int y0 = getTop();
    int x0 = getLeft();
    for (int y = getTop(); y < getTop() + getHeight(); y++) {
        for (int x = getLeft(); x < getLeft() + getWidth(); x++) {
            background[y - y0][x - x0] = &map.getEntityByCoords(0, 0);
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

}  // namespace Tanks::model
