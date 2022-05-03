#include "model/blocks.h"
#include <climits>
#include "constants.h"

namespace Tanks::model {
Block::Block(int left, int right) : Entity(left, right) {
}

int Block::getHeight() const {
    return TILE_SIZE;
}

int Block::getWidth() const {
    return TILE_SIZE;
}

EntityType Wall::getType() const {
    return EntityType::BRICK;
}

int Wall::getStrength() const {
    return 1;  // TODO constant
}

Wall::Wall(int left, int top) : Block(left, top) {
}

EntityType Steel::getType() const {
    return EntityType::STEEL;
}

int Steel::getStrength() const {
    return 4;  // TODO own constant
}

Steel::Steel(int left, int right) : Block(left, right) {
}

LevelBorder::LevelBorder(int left, int top, EntityType type_)
    : Block(left, top), type(type_) {
}

EntityType LevelBorder::getType() const {
    return type;
}

int LevelBorder::getStrength() const {
    return INT_MAX;  // TODO own constant
}

EntityType Floor::getType() const {
    return EntityType::FLOOR;
}

int Floor::getStrength() const {
    return 0;
}

Floor::Floor(int left, int right) : Block(left, right) {
}

bool Floor::isTankPassable() const {
    return true;
}

bool Floor::isBulletPassable() const {
    return true;
}

Water::Water(int left, int right) : Block(left, right) {
}

EntityType Water::getType() const {
    return EntityType::WATER;
}

bool Water::isBulletPassable() const {
    return true;
}

int Water::getStrength() const {
    return 0;
}
}  // namespace Tanks::model
