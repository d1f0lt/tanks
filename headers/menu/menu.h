#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "constants.h"
#include "menu_items.h"

namespace Tanks {

struct MenuController;

namespace Menu {

struct Menu final {
public:
    explicit Menu() = default;

    explicit Menu(size_t menuWidth,
                  const InscriptionInfo &titleInfo,
                  const InscriptionInfo &inscriptionsInfo,
                  const std::vector<ButtonWithType> &buttonsInfo);  // for most

    explicit Menu(size_t menuWidth,
                  const InscriptionInfo &titleInfo,
                  const std::vector<InscriptionInfo> &inscriptions,
                  const std::string &path,
                  size_t quantityPerLine,
                  Button btnInfo);  // for levels

    void draw(sf::RenderWindow &window) const;
    const MenuButton *showMenu(sf::RenderWindow &window,
                               const sf::Sprite &background);

    void addMenuItem(std::unique_ptr<MenuItem> &&item);
    void addIconToLeftUpCorner(const std::string &filename, ButtonType type);
    void addIconToLeftLowerCorner(const std::string &filename, ButtonType type);

    // animations
    void flyOutFromLeft(sf::RenderWindow &window,
                        const sf::Sprite &backgroundSprite);
    void flyAwayToLeft(sf::RenderWindow &window,
                       const sf::Sprite &backgroundSprite);
    void flyOutFromRight(sf::RenderWindow &window,
                         const sf::Sprite &backgroundSprite);
    void flyAwayToRight(sf::RenderWindow &window,
                        const sf::Sprite &backgroundSprite);
    void flyAwayToRight();  // without animation, to set the initial position
                            // for first animation
#ifdef MENU_TEST
    [[nodiscard]] const std::vector<std::unique_ptr<MenuItem>> &getItems()
        const;
#endif

private:
    std::vector<std::unique_ptr<MenuItem>> items{};
    constexpr static int animationSpeed = 4;

#ifndef MENU_TEST
    [[nodiscard]] const std::vector<std::unique_ptr<MenuItem>> &getItems()
        const;
#endif

    void animation(sf::RenderWindow &window,
                   const sf::Sprite &backgroundSprite,
                   size_t stepsAmount,
                   float speed_);

    void moveItems(float distance);

    friend struct Tanks::MenuController;
};

}  // namespace Menu

}  // namespace Tanks

#endif