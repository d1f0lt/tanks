#include "controller.h"
#include "menu.h"
#include "player.h"

namespace Tanks {

bool GameController::isEscReleased(const sf::Event &event) {
    return (event.type == sf::Event::KeyReleased &&
            event.key.code == sf::Keyboard::Escape);
}

void GameController::makeMove(Player &player, const double time) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.updatePosition(Direction::LEFT, time);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.updatePosition(Direction::RIGHT, time);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        player.updatePosition(Direction::DOWN, time);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        player.updatePosition(Direction::UP, time);
    }
}

std::optional<Menu::MenuButton *> MenuController::control(
    const std::vector<std::unique_ptr<Menu::MenuItem>> &items,
    sf::RenderWindow &window,
    const sf::Event &event) {
    for (auto &menuItem : items) {
        auto item = dynamic_cast<Menu::MenuButton *>(menuItem.get());
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
                event.mouseButton.button == sf::Mouse::Left) {
                return item;
            }
        }
    }
    return std::nullopt;
}

std::optional<Menu::MenuButton *> MenuController::control(const Menu::Menu &menu, sf::RenderWindow &window, const sf::Event &event) {
    const auto &items = menu.getItems();
    return control(items, window, event);
}

}  // namespace Tanks