#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "menu.h"
#include "player.h"

namespace Tanks {

struct GameController final {
public:
    static bool isEscReleased(const sf::Event &event);

    static void makeMove(Player &player, double time);
};

struct MenuController final {
public:
    static std::optional<Menu::MenuButton *> control(const std::vector<std::unique_ptr<Menu::MenuItem>> &items,
                                                     sf::RenderWindow &window,
                                                     const sf::Event &event);
    static std::optional<Menu::MenuButton *> control(const Menu::Menu &menu,
                                                     sf::RenderWindow &window,
                                                     const sf::Event &event);
};

}  // namespace Tanks

#endif  // CONTROLLER_H