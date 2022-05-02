#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <optional>
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
    static std::optional<Tanks::Menu::ButtonType> control(
        const Menu::Menu &menu,
        sf::RenderWindow &window);
};

}  // namespace Tanks

#endif  // CONTROLLER_H