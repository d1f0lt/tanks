#include "game_controller.h"
#include <cassert>

namespace Tanks {

namespace {

int getDistanceFromTank(model::PlayableTank &player, const model::Entity* const second, model::Direction dir) {
    int distance{};
    switch (dir) {
        case model::Direction::UP:
            distance = player.getTop() - second->getTop() - second->getHeight();
            break;
        case model::Direction::DOWN:
            distance = second->getTop() - player.getTop() - player.getHeight();
            break;
        case model::Direction::LEFT:
            distance = player.getLeft() - second->getLeft() - second->getWidth();
            break;
        case model::Direction::RIGHT:
            distance = second->getLeft() - player.getLeft() - player.getWidth();
            break;
    }
    assert(distance >= 0);
    return distance;
}

}

void GameController::makeMove(model::PlayableTank &player,
                              model::Direction direction) {
    player.setDirection(direction);
    int minDistance = player.getSpeed();
    auto items = player.look(direction);
    for (const auto *const item : items) {
        if (item->getType() != model::EntityType::FLOOR) {
            minDistance = std::min(minDistance, getDistanceFromTank(player, item, direction));
        }
    }
    player.move(direction, minDistance);
}

bool GameController::isEscReleased(const sf::Event &event) {
    return (event.type == sf::Event::KeyReleased &&
            event.key.code == sf::Keyboard::Escape);  // NOLINT
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