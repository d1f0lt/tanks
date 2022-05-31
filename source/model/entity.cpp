#include "model/entity.h"
#include <cassert>
#include <unordered_set>
#include "model/handler.h"

namespace Tanks::model {
int Entity::getLeft() const {
    return left_;
}

int Entity::getTop() const {
    return top_;
}

sf::IntRect Entity::getRect() const {
    return {left_, top_, getHeight(), getWidth()};
}

bool Entity::intersect(const Entity &other) const {
    return getRect().intersects(other.getRect());
}

void Entity::setTop(int top) {
    top_ = top;
}

void Entity::setLeft(int left) {
    left_ = left;
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

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
Entity::Entity(int left,
               int top,
               int entityId,
               std::unique_ptr<BasicHandler> handler)
    : left_(left), top_(top), id_(entityId), handler_(std::move(handler)) {
}

int Entity::getId() const {
    return id_;
}

BasicHandler &Entity::getHandler() const {
    return *handler_;
}

std::unique_ptr<BasicHandler> &Entity::getAccessToHandler() {
    return handler_;
}

bool Entity::canStandOn(const Entity &other) const {
    return getHandler().canStandOn(other);
}

IncrId::IncrId(const int data) : data(data) {
    assert(data >= 0);
}

IncrId::operator int() const {
    return data;
}

IncrId IncrId::operator++(int) {  // postfix
    IncrId res(data++);
    return res;
}

DecrId::DecrId(int data) : data(data) {
    assert(data < 0);
}

DecrId::operator int() const {
    return data;
}

DecrId DecrId::operator--(int) {  // postfix
    DecrId res(data--);
    return res;
}
}  // namespace Tanks::model
