#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "constants.h"

namespace Tanks {

struct MenuController;

namespace Menu {

enum class ButtonType { NEW_GAME, RESUME, SETTINGS, CREATE_MAP, RATING, EXIT, PAUSE };

std::string convertButtonTypeToString(ButtonType type);

struct MenuItem;

struct Menu final {
public:
    explicit Menu() = default;

    explicit Menu(const std::string &imagesPath,
                  int menuWidth,
                  const std::vector<ButtonType> &buttonTypes,
                  int buttonsHeight,
                  const sf::Color &buttonsStandardColor,
                  const sf::Color &buttonsHoverColor);  // TODO make struct for
                                                        // buttons parameters

    void drawMenu(sf::RenderWindow &window) const;

    void addMenuItem(std::unique_ptr<MenuItem> &&item);

private:
    std::vector<std::unique_ptr<MenuItem>> items;

    [[nodiscard]] const std::vector<std::unique_ptr<MenuItem>> &getItems()
        const;

    friend struct Tanks::MenuController;
};

struct MenuItem {
public:
    explicit MenuItem(const std::string &path,
                      const sf::Vector2<float> &coordinates);

    virtual void drawSprite(sf::RenderWindow &window) const;

protected:
    sf::Image inscriptionImage;
    sf::Texture inscriptionTexture;
    sf::Sprite inscriptionSprite;

    friend struct Menu;
};

struct MenuButton final : MenuItem {
public:
    MenuButton(const std::string &path,
               const sf::Vector2<float> &coordinates,
               const sf::Vector2<float> &rectangleSize,
               const sf::Color &rectangleColor_,
               const sf::Color &rectangleHoverColor_,
               ButtonType type_);

    void drawSprite(sf::RenderWindow &window) const final;

    ButtonType getType() const;

    void hover();

private:
    mutable sf::RectangleShape rectangle;  // we want drawSprite to be a const
    sf::Color rectangleStandardColor;
    sf::Color rectangleHoverColor;
    ButtonType type;

    friend struct Menu;
    friend struct Tanks::MenuController;
};

}  // namespace Menu

}  // namespace Tanks

#endif