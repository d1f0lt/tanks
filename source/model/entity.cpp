#include "entity.h"

namespace Tanks::model {
/*

int Entity::getId() const {
    return id;
}
*/

EntityType Entity::getType() const {
    return type;
}

sf::Rect<int> Entity::getRect() const {
    return rect;
}

int Entity::getWidth() const {
    return getRect().width;
}

int Entity::getHeight() const {
    return getRect().height;
}

int Entity::getLeft() const {
    return getRect().left;
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

Entity::Entity(int left, int top, int width, int height, EntityType type_)
    : type(type_), rect(left, top, width, height) {
}

/*
void Entity::setCoords(int x, int y) {
    rect.top = y;
    rect.left = x;
}
 */

}  // namespace Tanks::model
