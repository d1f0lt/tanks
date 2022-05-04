#include "model/blocks.h"
#include "constants.h"

namespace Tanks::model {
Block::Block(int left, int right, EntityType entityType)
    : Entity(left, right, TILE_SIZE, TILE_SIZE, entityType) {
}

}  // namespace Tanks::model
