#include "menu_controller.h"

namespace Tanks {

std::optional<Menu::MenuButton *> MenuController::control(
    const std::vector<std::unique_ptr<Menu::MenuItem>> &items,
    sf::RenderWindow &window,
    const sf::Event &event) {
    for (const auto &menuItem : items) {
        auto *item = dynamic_cast<Menu::MenuButton *>(menuItem.get());
        if (item == nullptr) {  // header
            continue;
        }
        auto coordinates = static_cast<sf::Vector2<int>>(item->getPosition());
        auto proportions = static_cast<sf::Vector2<int>>(item->getSize());
        if (sf::IntRect(coordinates.x, coordinates.y, proportions.x,
                        proportions.y)
                .contains(sf::Mouse::getPosition(window))) {
            item->hover();
            if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left) {  // NOLINT
                return item;
            }
        }
    }
    return std::nullopt;
}

std::optional<Menu::MenuButton *> MenuController::control(
    const Menu::Menu &menu,
    sf::RenderWindow &window,
    const sf::Event &event) {
    const auto &items = menu.getItems();
    return control(items, window, event);
}

}  // namespace Tanks