#include "blocks.h"
#include "constants.h"

namespace Tanks::model {
Block::Block(int col, int row, EntityType entityType)
    : Entity(col, row, TILE_SIZE, TILE_SIZE, entityType) {
}

bool Block::isTankPassable() const {
    // TODO
    return false;
}

bool Block::isBulletPassable() const {
    // TODO
    return false;
}

bool Block::isDestroyable() const {
    // TODO
    return false;
}
}  // namespace Tanks::model
