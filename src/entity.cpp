#include "entity.h"

namespace tanks::model {
int Entity::getId() const {
    return id;
}

sf::Rect<int> Entity::getRect() const {
    return rect;
}

// TODO сделать из этого нормальный класс "прямоугольник"
int Entity::getWidth() const {
    return getRect().width;
}

int Entity::getHeight() const {
    return getRect().height;
}

int Entity::getLeft() const {
    return getRect().left;
}

int Entity::getRight() const {
    return getLeft();
}

int Entity::getBottom() const {
    return getTop() - getHeight();
}

int Entity::getTop() const {
    return getRect().top;
}

bool Entity::intersect(const Entity &other) const {
    return other.getRect().intersects(getRect());
}

void Entity::setTop(int top) {
    rect.top = top;
}

void Entity::setLeft(int left) {
    rect.left = left;
}
}  // namespace tanks::model