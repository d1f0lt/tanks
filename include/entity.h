#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace tanks::model {
enum class Direction { UP, RIGHT, DOWN, LEFT };

class Entity {
public:
    [[nodiscard]] int getId() const;

    [[nodiscard]] sf::Rect<int> getRect() const;

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] int getLeft() const;

    [[nodiscard]] int getRight() const;

    [[nodiscard]] int getTop() const;

    [[nodiscard]] int getBottom() const;

    [[nodiscard]] bool intersect(const Entity &other) const;

protected:
    void setTop(int top);

    void setLeft(int left);

private:
    int id;
    sf::Rect<int> rect;
};

}  // namespace tanks::model
