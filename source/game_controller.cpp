#include "game_controller.h"
#include "sound/shoot_sound.h"

namespace Tanks {

bool GameController::isEscReleased(const sf::Event &event) {
    return (event.type == sf::Event::KeyReleased &&
            event.key.code == sf::Keyboard::Escape);  // NOLINT
}

void GameController::makeMove(model::PlayerActionsHandler &player,
                              model::Direction direction) {
    player.move(direction);
}

void GameController::makeMove(model::PlayerActionsHandler &player) {
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

void GameController::makeShot(model::PlayerActionsHandler &player) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        player.shoot();
    }
}

}  // namespace Tanks