#include "model/tank.h"
#include <cassert>
#include "model/game_map.h"

namespace Tanks::model {
void PlayableTank::move(Direction dir) {
    setDirection(dir);
    move_(dir);
}

PlayableTank::PlayableTank(int left, int top, GameMap &map_)
    : Tank(left, top, EntityType::PLAYABLE_TANK, map_) {
}

Direction Tank::getDirection() const {
    return direction;
}

Tank::Tank(int left, int top, EntityType type_, GameMap &map_)
    : ForegroundEntity(left, top, TANK_SIZE, TANK_SIZE, type_, map_) {
}

void Tank::setDirection(Direction dir) {
    direction = dir;
}

std::vector<const Entity *> Tank::whatsOn(Direction dir) {
    if (dir == Direction::DOWN) {
        if (getTop() + getHeight() == map.getHeight() - 1) {
            return {};
        }

        std::vector<const Entity *> res(getWidth());
        for (int col = getLeft(); col < getLeft() + getWidth(); col++) {
            res[col - getLeft()] =
                &map.getEntityByCoords(col, getTop() + getHeight() + 1);
        }
        return res;
    } else if (dir == Direction::UP) {
        if (getTop() == 0) {
            return {};
        }

        std::vector<const Entity *> res(getWidth());
        for (int col = getLeft(); col < getLeft() + getWidth(); col++) {
            res[col - getLeft()] = &map.getEntityByCoords(col, getTop() - 1);
        }
        return res;
    } else if (dir == Direction::RIGHT) {
        if (getLeft() + getWidth() == map.getWidth() - 1) {
            return {};
        }
        std::vector<const Entity *> res(getHeight());
        for (int row = getTop(); row < getTop() + getHeight(); row++) {
            res[row - getTop()] =
                &map.getEntityByCoords(getLeft() + getWidth() + 1, row);
        }
        return res;
    } else {
        assert(dir == Direction::LEFT);
        std::vector<const Entity *> res(getHeight());
        if (getLeft() == 0) {
            return {};
        }
        for (int row = getTop(); row < getTop() + getHeight(); row++) {
            res[row - getTop()] = &map.getEntityByCoords(getLeft() - 1, row);
        }
        return res;
    }
}

void Tank::shoot() {
}
}  // namespace Tanks::model
