#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <optional>
#include "menu.h"

namespace Tanks {

struct MenuController final {
public:
    static std::optional<Menu::MenuButton *> control(
        const std::vector<std::unique_ptr<Menu::MenuItem>> &items,
        sf::RenderWindow &window,
        const sf::Event &event);
    static std::optional<Menu::MenuButton *> control(const Menu::Menu &menu,
                                                     sf::RenderWindow &window,
                                                     const sf::Event &event);
};

}  // namespace Tanks

#endif  // CONTROLLER_H