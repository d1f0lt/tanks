#include "model/entity.h"
#include <unordered_set>

namespace Tanks::model {
Entity::Entity(int left, int top, int width, int height, EntityType type_)
    : type(type_), rect(left, top, width, height) {
}

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

bool Entity::isTankPassable() const {
    static const std::unordered_set<EntityType> PASSABLE = {
        EntityType::FLOOR, EntityType::GRASS, EntityType::BULLET};
    return PASSABLE.count(getType()) == 1;
}

bool Entity::isBulletPassable() const {
    static const std::unordered_set<EntityType> PASSABLE = {
        EntityType::GRASS, EntityType::FLOOR, EntityType::WATER};
    return PASSABLE.count(getType()) == 1;
}

bool Entity::isDestroyable() const {
    static const std::unordered_set<EntityType> DESTROYABLE = {
        EntityType::BRICK, EntityType::BULLET, EntityType::PLAYABLE_TANK,
        EntityType::BOT_TANK};
    return DESTROYABLE.count(getType()) == 1;
}
}  // namespace Tanks::model
