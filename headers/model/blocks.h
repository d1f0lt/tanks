#ifndef TANKS_BLOCKS_H
#define TANKS_BLOCKS_H

#include <climits>
#include "entity.h"
#include "model/game_model_fwd.h"

namespace Tanks::model {
enum class Strength {
    BRICK = 1,
    FLOOR = 0,
    LEVEL_BORDER = INT_MAX,
    WATER = 0,
    STEEL = 10
};

class Block : public Entity {
public:
    explicit Block(int left,
                   int top,
                   int entityId,
                   std::unique_ptr<BasicHandler> handler);

    [[nodiscard]] int getHeight() const final;
    [[nodiscard]] int getWidth() const final;
};

class Floor final : public Block {
public:
    explicit Floor(int left, int top, int entityId, GameModel &model);

    [[nodiscard]] EntityType getType() const override;
    [[nodiscard]] bool isTankPassable() const final;
    [[nodiscard]] bool isBulletPassable() const final;
    [[nodiscard]] int getStrength() const override;
};

class Water final : public Block {
public:
    explicit Water(int left, int top, int entityId, GameModel &model);

    [[nodiscard]] EntityType getType() const override;
    [[nodiscard]] bool isBulletPassable() const override;
    [[nodiscard]] int getStrength() const override;
};

class Brick final : public Block {
public:
    explicit Brick(int left, int top, int entityId, GameModel &model);

    [[nodiscard]] EntityType getType() const final;
    [[nodiscard]] int getStrength() const final;
};

class Steel final : public Block {
public:
    explicit Steel(int left, int top, int entityId, GameModel &model);

    [[nodiscard]] EntityType getType() const final;
    [[nodiscard]] int getStrength() const final;
};

class LevelBorder : public Block {
public:
    explicit LevelBorder(int left,
                         int top,
                         EntityType type,
                         int entityId,
                         GameModel &model);

    [[nodiscard]] EntityType getType() const final;
    [[nodiscard]] int getStrength() const final;

private:
    const EntityType type_;
};

}  // namespace Tanks::model

#endif  // TANKS_BLOCKS_H
