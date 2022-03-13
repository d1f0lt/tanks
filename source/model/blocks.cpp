#include "model/blocks.h"
#include <unordered_set>
#include "constants.h"

namespace Tanks::model {
Block::Block(int col, int row, EntityType entityType)
    : Entity(col, row, TILE_SIZE, TILE_SIZE, entityType) {
}

}  // namespace Tanks::model
