#include "game_controller.h"

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

}  // namespace Tanks