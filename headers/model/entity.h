#ifndef TANKS_ENTITY_H
#define TANKS_ENTITY_H

#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include "constants.h"
#include "model/handler_fwd.h"

namespace Tanks::model {
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
    MEDIUM_TANK,
    BULLET,
    GRASS,
    WALK_ON_WATER_BONUS
};

class Entity {
    friend WalkOnWaterHandler;
    //    friend ForegroundHandler;

public:
    explicit Entity(int left,
                    int top,
                    int entityId,
                    std::unique_ptr<BasicHandler> handler);

    Entity(const Entity &) = delete;
    Entity(Entity &&) = delete;
    Entity &operator=(const Entity &) = delete;
    Entity &operator=(Entity &&) = delete;

    virtual ~Entity() = default;

    [[nodiscard]] virtual EntityType getType() const = 0;
    [[nodiscard]] int getId() const;

    [[nodiscard]] int getLeft() const;
    [[nodiscard]] int getTop() const;
    [[nodiscard]] virtual int getWidth() const = 0;
    [[nodiscard]] virtual int getHeight() const = 0;
    [[nodiscard]] virtual int getStrength() const = 0;

    [[nodiscard]] bool intersect(const Entity &other) const;
    [[nodiscard]] int dist(const Entity &other) const;

    [[nodiscard]] virtual bool isTankPassable() const;
    [[nodiscard]] virtual bool isBulletPassable() const;
    [[nodiscard]] bool canPass(const Entity &other) const;

protected:
    void setTop(int top);
    void setLeft(int left);

    [[nodiscard]] BasicHandler &getHandler() const;

private:
    [[nodiscard]] sf::IntRect getRect() const;
    [[nodiscard]] std::unique_ptr<BasicHandler> &getAccessToHandler();

    int left_ = -1, top_ = -1;
    const int id_ = -1;
    std::unique_ptr<BasicHandler> handler_;
};
}  // namespace Tanks::model

#endif  // TANKS_ENTITY_H
