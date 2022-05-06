#include "model/blocks.h"
#include <climits>
#include "constants.h"

namespace Tanks::model {
Block::Block(int left, int top) : Entity(left, top) {
}

int Block::getHeight() const {
    return TILE_SIZE;
}

int Block::getWidth() const {
    return TILE_SIZE;
}

bool Block::canPass(const Entity &) const {
    return false;
}

EntityType Brick::getType() const {
    return EntityType::BRICK;
}

int Brick::getStrength() const {
    return static_cast<int>(Strength::BRICK);
}

Brick::Brick(int left, int top) : Block(left, top) {
}

EntityType Steel::getType() const {
    return EntityType::STEEL;
}

int Steel::getStrength() const {
    return static_cast<int>(Strength::STEEL);
}

Steel::Steel(int left, int top) : Block(left, top) {
}

LevelBorder::LevelBorder(int left, int top, EntityType type)
    : Block(left, top), type_(type) {
}

EntityType LevelBorder::getType() const {
    return type_;
}

int LevelBorder::getStrength() const {
    return static_cast<int>(Strength::LEVEL_BORDER);
}

EntityType Floor::getType() const {
    return EntityType::FLOOR;
}

int Floor::getStrength() const {
    return static_cast<int>(Strength::FLOOR);
}

Floor::Floor(int left, int top) : Block(left, top) {
}

bool Floor::isTankPassable() const {
    return true;
}

bool Floor::isBulletPassable() const {
    return true;
}

Water::Water(int left, int top) : Block(left, top) {
}

EntityType Water::getType() const {
    return EntityType::WATER;
}

bool Water::isBulletPassable() const {
    return true;
}

int Water::getStrength() const {
    return static_cast<int>(Strength::WATER);
}
}  // namespace Tanks::model
