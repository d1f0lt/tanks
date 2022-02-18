#pragma once

#include <array>
#include "entity.h"

namespace tanks::model {
enum class BlockType { GROUND, GRASS, RED_WALL, METAL_WALL, WATER };

class Block : public Entity {
public:
    [[nodiscard]] BlockType getType() const;

private:
    BlockType type;
};
}  // namespace tanks::model
