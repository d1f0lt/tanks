#include "game.h"
#include <chrono>
#include <thread>
#include "constants.h"
#include "controller.h"
#include "game_environment.h"
#include "map.h"
#include "movable_object_view.h"
#include "pause.h"
#include "player.h"

namespace Tanks {

namespace {

// void bullets_control(Player &player, std::list<Bullet>
// &bullets, double time, sf::RenderWindow &window) {
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
//        player.is_have_shot()) {
//        bullets.emplace_back(Bullet(player.getDirection(),
//                                           player.getCoordinates()));
//        player.make_shot();
//    }
//
//    for (auto iterator = bullets.begin(); iterator != bullets.end();) {
//        if (!iterator->isDestroyed()) {
//            iterator->updatePosition(iterator->getDirection(), time);
//            window.draw(iterator->get_bullet_sprite());
//            iterator->checkIntersectionWithMap();
//            iterator++;
//        }
//    }
//    // needs to be redone
//    bullets.remove_if([&](Bullet &bullet) {
//        if (bullet.isDestroyed()) {
//            player.recover_bullet();
//            return true;
//        }
//        return false;
//    });
//}

sf::Sprite initBackground(const std::string &path) {
    static const std::string imageFilename =
        path + "environment/background.png";
    sf::Image image;
    image.loadFromFile(imageFilename);
    static sf::Texture texture;  // so that the texture isn't
                                 // destroyed after the function exits
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(0, 0);
    return sprite;
}

}  // namespace

std::optional<Menu::ButtonType> startGame(sf::RenderWindow &window, int level) {
    static const std::string imagesPath = "../images/";
    static const std::string tankImageFilename =
        imagesPath + "tanks/player_tanks.png";
    static const std::string mapImageFilename = imagesPath + "map.png";

    Player player(sf::Vector2<int>(
        MARGIN_LEFT + 7 * TILE_SIZE,
        TILE_SIZE * (MAP_HEIGHT - 2) + MARGIN_TOP + TILE_SIZE - TANK_SIZE));

    MovableView playerView(player, tankImageFilename);

    // map
    static Map map(mapImageFilename);
    map.loadLevel(level);

    static sf::Sprite backgroundSprite = initBackground(imagesPath);

    Environment environment(imagesPath + "environment/");

    sf::Clock clock;

    Pause pause;

    //    std::list<Bullet> bullets; TODO

    while (window.isOpen()) {
        // calculate time since last tick
        double time =
            static_cast<double>(clock.getElapsedTime().asMicroseconds());
        clock.restart();
        time = time / 800;

        // catch event
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        pause.checkPause(event);

        if (!pause.isPause()) {
            if (auto signal =
                    MenuController::control(environment.getMenu(), window, event);
                signal != std::nullopt) {
                assert(signal == Menu::ButtonType::PAUSE);
                pause.makePause();
            } else {
                GameController::makeMove(player, time);
                player.checkCollisionWithMap(map, time);
            }
        }
        //        bullets_control(player, bullets, time, window); TODO

        // redraw
        window.clear();
        window.draw(backgroundSprite);
        environment.draw(window, pause);

        map.drawMap(window);

        window.draw(playerView.getSprite());

        if (pause.isPause()) {
            if (auto signal = MenuController::control(pause.getMenu(), window, event);
                signal != std::nullopt) {
                switch (signal.value()) {
                    case Menu::ButtonType::RESUME:
                        pause.unpause();
                        break;
                    case Menu::ButtonType::NEW_GAME:
                        return Menu::ButtonType::NEW_GAME;
                    case Menu::ButtonType::EXIT:
                        return signal.value();
                    default:
                        break;
                }
            }
            pause.drawPause(window);
        }

        window.display();

        // Hack for better performance
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    return std::nullopt;
}

}  // namespace Tanks