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
    explicit Entity(int left_,
                    int top_,
                    int height_,
                    int width_,
                    EntityType type_);

    Entity(const Entity &) = delete;
    Entity(Entity &&) = delete;
    Entity &operator=(const Entity &) = delete;
    Entity &operator=(Entity &&) = delete;

    virtual ~Entity() = default;

    [[nodiscard]] EntityType getType() const;

    [[nodiscard]] int getLeft() const;
    [[nodiscard]] int getTop() const;

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;

    [[nodiscard]] bool intersect(const Entity &other) const;

    // TODO: make it pure virtual and override in derives
    [[nodiscard]] bool isTankPassable() const;
    [[nodiscard]] bool isBulletPassable() const;
    [[nodiscard]] bool isDestroyable() const;

protected:
    void setTop(int top);
    void setLeft(int left);

private:
    [[nodiscard]] sf::Rect<int> getRect() const;

    EntityType type;
    sf::Rect<int> rect = {-1, -1, -1, -1};
};
}  // namespace Tanks::model

#endif  // TANKS_ENTITY_H
