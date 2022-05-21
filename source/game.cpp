#include <cassert>
#include <chrono>
#include <thread>
#include "game_controller.h"
#include "game_environment.h"
#include "menu/menu_controller.h"
#include "model/game_model.h"
#include "pause.h"
#include "view/bullets_view.h"
#include "view/game_view.h"
#include "view/tank_view.h"

namespace Tanks {

namespace {

model::PlayableTank &getPlayerTank(
    std::optional<std::reference_wrapper<model::Entity>> &player) {
    return dynamic_cast<model::PlayableTank &>(player.value().get());
}

void makeAction(std::optional<std::reference_wrapper<model::Entity>> &player) {
    if (player.has_value()) {
        auto &playerTank = getPlayerTank(player);
        GameController::makeShot(playerTank);
        GameController::makeMove(playerTank);
    }
}

}  // namespace

std::optional<Menu::ButtonType>
startGame(  // NOLINT(readability-function-cognitive-complexity)
    sf::RenderWindow &window,
    int level) {
    static const std::string imagesPath = "../images/";
    const sf::Vector2<int> tankStartCoordinates = {
        TILE_SIZE * 6 + (TILE_SIZE - TANK_SIZE) / 2,
        TILE_SIZE * (MAP_HEIGHT - 2) + (TILE_SIZE - TANK_SIZE)};

    model::GameModel model;
    model.loadLevel(level);
    const auto playerId =
        model.spawnPlayableTank(tankStartCoordinates.x, tankStartCoordinates.y)
            .getId();

    View::TankSpriteHolder greenTankView(imagesPath + "tanks/green_tank.png");

    View::BulletsSpriteHolder bulletsView(imagesPath + "bullet.png");

    View::Map mapView(imagesPath + "map.png", level);

    Environment environment(imagesPath + "environment/");

    Pause pause;

    while (window.isOpen()) {
        // catch event
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        pause.checkPause(event);

        auto player = model.getById(playerId);

        if (!pause.isPause()) {
            if (auto signal = MenuController::control(environment.getMenu(),
                                                      window, event);
                signal != std::nullopt) {
                assert(signal.value()->getType() == Menu::ButtonType::PAUSE);
                pause.makePause();
            } else {
                model.nextTick();
                player = model.getById(playerId);
                makeAction(player);
            }
        } else {
            if (auto signal =
                    MenuController::control(pause.getMenu(), window, event);
                signal != std::nullopt) {
                switch (signal.value()->getType()) {
                    case Menu::ButtonType::RESUME:
                        pause.unpause();
                        break;
                    case Menu::ButtonType::NEW_GAME:
                        return Menu::ButtonType::NEW_GAME;
                    case Menu::ButtonType::EXIT:
                        return signal.value()->getType();
                    default:
                        break;
                }
            }
        }

        // redraw
        window.clear();
        environment.draw(window, pause.isPause());

        mapView.draw(window, model);

        if (player.has_value()) {
            auto &playerTank = getPlayerTank(player);
            greenTankView.draw(window, playerTank);
        }

        const auto &bullets = model.getAll(model::EntityType::BULLET);
        bulletsView.draw(window, bullets);

        if (pause.isPause()) {
            pause.drawPause(window);
        }

        window.display();

        // Hack for better performance
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    return std::nullopt;
}

}  // namespace Tanks