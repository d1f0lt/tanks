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

std::optional<Menu::ButtonType> MenuController::control(
    const Menu::Menu &menu,
    sf::RenderWindow &window,
    const sf::Event &event) {
    auto &items = menu.getItems();
    for (auto &menuItem : items) {
        auto item = dynamic_cast<Menu::MenuButton *>(menuItem.get());
        if (item == nullptr) {  // header
            continue;
        }
        auto coordinates =
            static_cast<sf::Vector2<int>>(item->rectangle.getPosition());
        auto proportions =
            static_cast<sf::Vector2<int>>(item->rectangle.getSize());
        if (sf::IntRect(coordinates.x, coordinates.y, proportions.x,
                        proportions.y)
                .contains(sf::Mouse::getPosition(window))) {
            item->hover();
            if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left) {
                return item->getType();
            }
        }
    }
    return std::nullopt;
}

}  // namespace Tanks