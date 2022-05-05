#include "model/entity.h"
#include <unordered_set>

namespace Tanks::model {
// TODO better random
Entity::Entity(int left_, int top_) : left(left_), top(top_), id(rand()) {
}

int Entity::getLeft() const {
    return left;
}

int Entity::getTop() const {
    return top;
}

sf::IntRect Entity::getRect() const {
    return {left, top, getHeight(), getWidth()};
}

bool Entity::intersect(const Entity &other) const {
    return getRect().intersects(other.getRect());
}

void Entity::setTop(int top_) {
    top = top_;
}

void Entity::setLeft(int left_) {
    left = left_;
}

bool Entity::isTankPassable() const {
    return false;
}

bool Entity::isBulletPassable() const {
    return false;
}

namespace {
[[nodiscard]] int segdist(int left1, int right1, int left2, int right2) {
    if (left1 > left2) {
        std::swap(left1, left2);
        std::swap(right1, right2);
    }

    return std::max(0, left2 - right1);
}
}  // namespace

int Entity::dist(const Entity &other) const {
    int delta_x =
        segdist(getLeft(), getLeft() + getWidth() - 1, other.getLeft(),
                other.getLeft() + other.getWidth() - 1);
    int delta_y = segdist(getTop(), getTop() + getHeight() - 1, other.getTop(),
                          other.getTop() + other.getHeight() - 1);
    return delta_x + delta_y;
}

Entity::Entity(int left, int top, int id) : left(left), top(top), id(id) {
}
}  // namespace Tanks::model
