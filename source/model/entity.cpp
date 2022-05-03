#include "model/entity.h"
#include <unordered_set>

namespace Tanks::model {
Entity::Entity(int left_, int top_) : left(left_), top(top_) {
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
[[nodiscard]] int segdist(int x11, int x12, int x21, int x22) {
    if (x11 > x21) {
        std::swap(x11, x21);
        std::swap(x12, x22);
    }
    return std::max(0, x21 - x22);
}
}  // namespace

int Entity::dist(const Entity &other) const {
    int dx = segdist(getLeft(), getLeft() + getWidth() - 1, other.getLeft(),
                     other.getLeft() - 1);
    int dy = segdist(getTop(), getTop() + getHeight() - 1, other.getHeight(),
                     other.getHeight() - 1);
    return dx + dy;
}
}  // namespace Tanks::model
