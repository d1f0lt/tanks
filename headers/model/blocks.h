#ifndef TANKS_BLOCKS_H
#define TANKS_BLOCKS_H

#include "entity.h"

namespace Tanks::model {

class Block : public Entity {
public:
    explicit Block(int left, int right);

    [[nodiscard]] int getHeight() const final;
    [[nodiscard]] int getWidth() const final;
};

class Floor final : public Block {
public:
    explicit Floor(int left, int right);

    [[nodiscard]] EntityType getType() const override;
    [[nodiscard]] bool isTankPassable() const final;
    [[nodiscard]] bool isBulletPassable() const final;
    [[nodiscard]] int getStrength() const override;
};

class Water final : public Block {
public:
    explicit Water(int left, int right);
    [[nodiscard]] EntityType getType() const override;
    [[nodiscard]] bool isBulletPassable() const override;
    [[nodiscard]] int getStrength() const override;
};

class Wall final : public Block {
public:
    explicit Wall(int left, int top);

    [[nodiscard]] EntityType getType() const final;
    [[nodiscard]] int getStrength() const final;
};

class Steel final : public Block {
public:
    explicit Steel(int left, int right);

    [[nodiscard]] EntityType getType() const final;
    [[nodiscard]] int getStrength() const final;
};

class LevelBorder : public Block {
public:
    explicit LevelBorder(int left, int top, EntityType type_);

    [[nodiscard]] EntityType getType() const final;
    [[nodiscard]] int getStrength() const final;

private:
    EntityType type;
};

}  // namespace Tanks::model

#endif  // TANKS_BLOCKS_H
