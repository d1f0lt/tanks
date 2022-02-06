#include "game.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include "constants.h"
#include "movable_object_view.h"
#include "player.h"

namespace Tanks {

namespace {

void makeMove(Tanks::Player &player, double time) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.updatePosition(Tanks::Direction::LEFT, time);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.updatePosition(Tanks::Direction::RIGHT, time);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        player.updatePosition(Tanks::Direction::DOWN, time);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        player.updatePosition(Tanks::Direction::UP, time);
    }
}

void drawMap(sf::RenderWindow &window,
             const std::vector<std::vector<std::string>> &map,
             sf::Sprite &mapSprite) {
    for (int i = 0; i < Tanks::MAP_HEIGHT; ++i) {
        for (int j = 0; j < Tanks::MAP_WIDTH; ++j) {
            std::string item = map[i][j];
            if (item == " ") {
                mapSprite.setTextureRect(
                    sf::IntRect(2 * Tanks::TILE_SIZE, Tanks::TILE_SIZE,
                                Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "=") {
                mapSprite.setTextureRect(sf::IntRect(
                    Tanks::TILE_SIZE, 0, Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "|") {
                mapSprite.setTextureRect(sf::IntRect(
                    0, Tanks::TILE_SIZE, Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "1") {
                mapSprite.setTextureRect(
                    sf::IntRect(Tanks::TILE_SIZE, Tanks::TILE_SIZE,
                                Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "0") {
                mapSprite.setTextureRect(
                    sf::IntRect(Tanks::TILE_SIZE, 2 * Tanks::TILE_SIZE,
                                Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "~") {
                mapSprite.setTextureRect(sf::IntRect(3 * Tanks::TILE_SIZE, 0,
                                                     Tanks::TILE_SIZE,
                                                     Tanks::TILE_SIZE));
            } else if (item == "[") {
                mapSprite.setTextureRect(
                    sf::IntRect(0, 0, Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "]") {
                mapSprite.setTextureRect(sf::IntRect(2 * Tanks::TILE_SIZE, 0,
                                                     Tanks::TILE_SIZE,
                                                     Tanks::TILE_SIZE));
            } else if (item == "{") {
                mapSprite.setTextureRect(sf::IntRect(0, 2 * Tanks::TILE_SIZE,
                                                     Tanks::TILE_SIZE,
                                                     Tanks::TILE_SIZE));
            } else if (item == "}") {
                mapSprite.setTextureRect(
                    sf::IntRect(2 * Tanks::TILE_SIZE, 2 * Tanks::TILE_SIZE,
                                Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            }
            mapSprite.setPosition(
                static_cast<float>(Tanks::MARGIN_LEFT + Tanks::TILE_SIZE * j),
                static_cast<float>(Tanks::MARGIN_TOP + Tanks::TILE_SIZE * i));

            window.draw(mapSprite);
        }
    }
}

// void bullets_control(Tanks::Player &player, std::list<Tanks::Bullet>
// &bullets, double time, sf::RenderWindow &window) {
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
//        player.is_have_shot()) {
//        bullets.emplace_back(Tanks::Bullet(player.getDirection(),
//                                           player.getCoordinates()));
//        player.make_shot();
//    }
//
//    for (auto iterator = bullets.begin(); iterator != bullets.end();) {
//        if (!iterator->isDestroyed()) {
//            iterator->updatePosition(iterator->getDirection(), time);
//            window.draw(iterator->get_bullet_sprite());
////            iterator->checkIntersectionWithMap();
//            iterator++;
//        }
//    }
//    // needs to be redone
//    bullets.remove_if([&](Tanks::Bullet &bullet) {
//        if (bullet.isDestroyed()) {
//            player.recover_bullet();
//            return true;
//        }
//        return false;
//    });
//}

std::vector<std::vector<std::string>> parseCsv(int level) {
    std::fstream file("../levels/level" + std::to_string(level) + ".csv");
    std::vector<std::vector<std::string>> ans(
        Tanks::MAP_HEIGHT, std::vector<std::string>(Tanks::MAP_WIDTH));
    std::string str;
    for (int i = 0; i < Tanks::MAP_HEIGHT; ++i) {
        std::getline(file, str);
        for (int j = 0; j < Tanks::MAP_WIDTH * 2 - 1; j += 2) {
            ans[i][j / 2] = str[j];
        }
    }
    return ans;
}
}  // namespace

void startGame(sf::RenderWindow &window, int level) {
    static const std::string tankImageFilename =
        "../images/tanks/player_tanks.png";  // TODO remake initialization

    Tanks::Player player(
        //        "../images/tanks/player_tanks.png",
        sf::Vector2<int>(Tanks::MARGIN_LEFT + Tanks::TILE_SIZE,
                         Tanks::TILE_SIZE * (Tanks::MAP_HEIGHT - 2) +
                             Tanks::MARGIN_TOP * 3));

    Tanks::MovableView playerView(player, tankImageFilename);

    // map
    sf::Image mapImage;
    mapImage.loadFromFile("../images/map.png");
    sf::Texture mapTexture;
    mapTexture.loadFromImage(mapImage);
    sf::Sprite mapSprite;
    mapSprite.setTexture(mapTexture);
    std::vector<std::vector<std::string>> map = parseCsv(level);

    // background
    sf::Image backgroundImage;
    backgroundImage.loadFromFile("../images/environment/background.png");
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromImage(backgroundImage);
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);

    sf::Clock clock;

    //    std::list<Tanks::Bullet> bullets; TODO

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

        makeMove(player, time);

        // redraw
        window.clear();
        window.draw(backgroundSprite);

        drawMap(window, map, mapSprite);

        //        bullets_control(player, bullets, time, window); TODO

        //        player.check_interaction_with_map(); TODO

        window.draw(playerView.getSprite());
        window.display();

        // Hack for better performance
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

}  // namespace Tanks