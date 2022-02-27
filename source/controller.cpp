#include "controller.h"

namespace Tanks {

bool GameController::isEscReleased(const sf::Event &event) {
    return (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape);
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

bool PauseController::checkMouse(Pause &pause, sf::RenderWindow &window) { // TODO rename
    auto &items = pause.getItems();
    for (int i = 1; i < items.size(); ++i) {
        auto item = dynamic_cast<PauseButton *>(items[i].get());
        auto coordinates = static_cast<sf::Vector2<int>>(item->rectangle.getPosition());
        auto proportions = static_cast<sf::Vector2<int>>(item->rectangle.getSize());
        if (sf::IntRect(coordinates.x, coordinates.y, proportions.x, proportions.y).contains(sf::Mouse::getPosition(window))) {
            item->hover();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                switch (i) {  // TODO remake
                    case 1:
                        pause.pause = false;
                        break;
                    case 4:
                        return true;
                    default:
                        break;
                }
            }
        }
    }
    return false;;
}

}  // namespace Tanks