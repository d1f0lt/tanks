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
}  // namespace Tanks::model
