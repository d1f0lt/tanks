#include "controller.h"
#include "menu.h"
#include <cassert>

namespace Tanks {

void GameController::makeMove(model::PlayableTank &player, model::Direction direction) {
    for (auto &col : player.look(direction)) {
        if (col != nullptr && col->getType() != model::EntityType::FLOOR) {
            return;
        }
    }
    player.move(direction);
}

bool GameController::isEscReleased(const sf::Event &event) {
    return (event.type == sf::Event::KeyReleased &&
            event.key.code == sf::Keyboard::Escape);
}

void GameController::makeMove(model::PlayableTank &player) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        auto direction = model::Direction::LEFT;
        makeMove(player, direction);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        auto direction = model::Direction::RIGHT;
        makeMove(player, direction);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        auto direction = model::Direction::DOWN;
        makeMove(player, direction);
    } else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::W))) {
        auto direction = model::Direction::UP;
        makeMove(player, direction);
    }
}

std::optional<Menu::ButtonType> MenuController::control(
    const Menu::Menu &menu,
    sf::RenderWindow &window) {
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
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                return item->getType();
            }
        }
    }
    return std::nullopt;
}

}  // namespace Tanks