#include "controller.h"

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

std::optional<Button> PauseController::control(Pause &pause,
                                               sf::RenderWindow &window) {
    auto &items = pause.getItems();
    static std::unordered_map<int, Button> translateIntToButtonType{
        {1, Button::RESUME},
        {2, Button::NEW_GAME},
        {3, Button::SETTINGS},
        {4, Button::EXIT}};
    for (int i = 0; i < items.size(); ++i) {
        auto item = dynamic_cast<PauseButton *>(items[i].get());
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

                Button button = translateIntToButtonType[i];
                switch (button) {
                    case Button::RESUME:
                        pause.pause = false;
                        break;
                    case Button::NEW_GAME:
                        break;

                    case Button::SETTINGS:
                        break;

                    case Button::EXIT:
                        break;
                }
                return button;
            }
        }
    }
    return std::nullopt;
}

}  // namespace Tanks