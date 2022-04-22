#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "constants.h"

namespace Tanks {

struct MenuController;

namespace Menu {

enum class ButtonType { NEW_GAME, RESUME, SETTINGS, CREATE_MAP, RATING, EXIT, PAUSE, UPGRADE };

std::string convertButtonTypeToString(ButtonType type);

struct MenuItem;

struct Menu final {
public:
    explicit Menu() = default;

    explicit Menu(int menuWidth,
                  const std::vector<ButtonType> &buttonTypes,
                  int buttonsHeight,
                  const sf::Color &buttonsStandardColor,
                  const sf::Color &buttonsHoverColor);  // TODO make struct for
                                                        // buttons parameters

    void draw(sf::RenderWindow &window) const;

    void addMenuItem(std::unique_ptr<MenuItem> &&item);

private:
    std::vector<std::unique_ptr<MenuItem>> items;

    [[nodiscard]] const std::vector<std::unique_ptr<MenuItem>> &getItems()
        const;

    friend struct Tanks::MenuController;
};

struct MenuItem {
    virtual void draw(sf::RenderWindow &window) const = 0;

    [[nodiscard]] virtual sf::Vector2<int> getSize() const = 0;

    [[nodiscard]] virtual sf::Vector2<int> getPosition() const = 0;

    virtual void setPosition(sf::Vector2<int> newPosition) = 0;

    virtual ~MenuItem() = default;
};

struct MenuInscription : MenuItem {
    explicit MenuInscription(const std::string &inscription,
                         const sf::Vector2<float> &coordinates);

    sf::Vector2<int> getSize() const final;

    sf::Vector2<int> getPosition() const final;

    void setPosition(sf::Vector2<int> newPosition) final;

    void draw(sf::RenderWindow &window) const final;

protected:
    sf::Font font;
    sf::Text text;

    friend struct Menu;
};

struct MenuPicture : MenuItem {
public:
    explicit MenuPicture(const std::string &path,
                      const sf::Vector2<float> &coordinates);

    sf::Vector2<int> getSize() const final;

    sf::Vector2<int> getPosition() const final;

    void setPosition(sf::Vector2<int> newPosition) final;

    void draw(sf::RenderWindow &window) const final;

private:
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    friend struct Menu;
};

struct MenuButton final : MenuItem {
public:
    MenuButton(std::unique_ptr<MenuItem> &&content,
               const sf::Vector2<float> &coordinates,
               const sf::Vector2<float> &rectangleSize,
               const sf::Color &rectangleColor_,
               const sf::Color &rectangleHoverColor_,
               ButtonType type_);

    void draw(sf::RenderWindow &window) const final;

    ButtonType getType() const;

    void hover();

    sf::Vector2<int> getSize() const final;

    sf::Vector2<int> getPosition() const final;

    void setPosition(sf::Vector2<int> newPosition) final;

private:
    std::unique_ptr<MenuItem> content;
    mutable sf::RectangleShape rectangle;  // we want draw to be a const
    sf::Color rectangleStandardColor;
    sf::Color rectangleHoverColor;
    ButtonType type;

    friend struct Menu;
    friend struct Tanks::MenuController;
};

}  // namespace Menu

}  // namespace Tanks

#endif