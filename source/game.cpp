#include "game.h"
#include <chrono>
#include <thread>
#include "constants.h"
#include "controller.h"
#include "map.h"
#include "movable_object_view.h"
#include "player.h"
#include "pause.h"

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

}  // namespace

void startGame(sf::RenderWindow &window, int level) {
    static const std::string tankImageFilename =
        "../images/tanks/player_tanks.png";  // TODO remake initialization
    static const std::string mapImageFilename = "../images/map.png";
    static const std::string backgroundImageFilename =
        "../images/environment/background.png";

    Player player(sf::Vector2<int>(
        MARGIN_LEFT + 7 * TILE_SIZE,
        TILE_SIZE * (MAP_HEIGHT - 2) + MARGIN_TOP + TILE_SIZE - TANK_SIZE));

    MovableView playerView(player, tankImageFilename);

    // map
    static Map map(mapImageFilename);
    map.loadLevel(level);

    // background
    sf::Image backgroundImage;
    backgroundImage.loadFromFile(backgroundImageFilename);
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromImage(backgroundImage);
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);

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
            pause.checkPause(event);
        }

        if (!pause.isPause()) {
            GameController::makeMove(player, time);
            player.checkCollisionWithMap(map, time);
        }
        //        bullets_control(player, bullets, time, window); TODO

        // redraw
        window.clear();
        window.draw(backgroundSprite);

        map.drawMap(window);

        window.draw(playerView.getSprite());

        if (pause.isPause()) {
            pause.drawPause(window);
        }

        window.display();

        // Hack for better performance
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

}  // namespace Tanks