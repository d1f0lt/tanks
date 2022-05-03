#ifndef TANKS_ENTITY_H
#define TANKS_ENTITY_H

#include <SFML/Graphics/Rect.hpp>

namespace Tanks::model {
enum class Direction { UP, RIGHT, DOWN, LEFT };

enum class EntityType {
    LEFT_UP_CORNER,
    RIGHT_UP_CORNER,
    LEFT_DOWN_CORNER,
    RIGHT_DOWN_CORNER,
    VERTICAL_BORDER,
    HORIZONTAL_BORDER,
    FLOOR,
    BRICK,
    STEEL,
    WATER,
    PLAYABLE_TANK,
    BOT_TANK,
    BULLET,
    GRASS,
};

class Entity {
public:
    explicit Entity(int left_, int top_);

    Entity(const Entity &) = delete;
    Entity(Entity &&) = delete;
    Entity &operator=(const Entity &) = delete;
    Entity &operator=(Entity &&) = delete;

    virtual ~Entity() = default;

    [[nodiscard]] virtual EntityType getType() const = 0;
    [[nodiscard]] int getLeft() const;
    [[nodiscard]] int getTop() const;
    [[nodiscard]] virtual int getWidth() const = 0;
    [[nodiscard]] virtual int getHeight() const = 0;

    [[nodiscard]] bool intersect(const Entity &other) const;

    // TODO: make it pure virtual and override in derives
    [[nodiscard]] virtual bool isTankPassable() const;
    [[nodiscard]] virtual bool isBulletPassable() const;
    [[nodiscard]] virtual int getStrength() const = 0;

protected:
    void setTop(int top);
    void setLeft(int left);

private:
    [[nodiscard]] sf::IntRect getRect() const;

    int left, top;
};
}  // namespace Tanks::model

#endif  // TANKS_ENTITY_H
