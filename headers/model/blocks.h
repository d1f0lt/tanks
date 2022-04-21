#ifndef TANKS_BLOCKS_H
#define TANKS_BLOCKS_H

#include "entity.h"

namespace Tanks::model {

class Block : public Entity {
public:
    explicit Block(int col, int row, EntityType entityType);

private:
};
}  // namespace Tanks::model

#endif  // TANKS_BLOCKS_H
