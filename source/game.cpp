#include "game.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include "constants.h"
#include "player.h"
#include "movable_object_view.h"

namespace Tanks {

namespace {

void make_move(Tanks::Player &player, double time) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.update_position(Tanks::Direction::LEFT, time);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.update_position(Tanks::Direction::RIGHT, time);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        player.update_position(Tanks::Direction::DOWN, time);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        player.update_position(Tanks::Direction::UP, time);
    }
}

void draw_map(sf::RenderWindow &window,
              const std::vector<std::vector<std::string>> &map,
              sf::Sprite &map_sprite) {
    for (int i = 0; i < Tanks::MAP_HEIGHT; ++i) {
        for (int j = 0; j < Tanks::MAP_WIDTH; ++j) {
            std::string item = map[i][j];
            if (item == " ") {
                map_sprite.setTextureRect(
                    sf::IntRect(2 * Tanks::TILE_SIZE, Tanks::TILE_SIZE,
                                Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "=") {
                map_sprite.setTextureRect(sf::IntRect(
                    Tanks::TILE_SIZE, 0, Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "|") {
                map_sprite.setTextureRect(sf::IntRect(
                    0, Tanks::TILE_SIZE, Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "1") {
                map_sprite.setTextureRect(
                    sf::IntRect(Tanks::TILE_SIZE, Tanks::TILE_SIZE,
                                Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "0") {
                map_sprite.setTextureRect(
                    sf::IntRect(Tanks::TILE_SIZE, 2 * Tanks::TILE_SIZE,
                                Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "~") {
                map_sprite.setTextureRect(sf::IntRect(3 * Tanks::TILE_SIZE, 0,
                                                      Tanks::TILE_SIZE,
                                                      Tanks::TILE_SIZE));
            } else if (item == "[") {
                map_sprite.setTextureRect(
                    sf::IntRect(0, 0, Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            } else if (item == "]") {
                map_sprite.setTextureRect(sf::IntRect(2 * Tanks::TILE_SIZE, 0,
                                                      Tanks::TILE_SIZE,
                                                      Tanks::TILE_SIZE));
            } else if (item == "{") {
                map_sprite.setTextureRect(sf::IntRect(0, 2 * Tanks::TILE_SIZE,
                                                      Tanks::TILE_SIZE,
                                                      Tanks::TILE_SIZE));
            } else if (item == "}") {
                map_sprite.setTextureRect(
                    sf::IntRect(2 * Tanks::TILE_SIZE, 2 * Tanks::TILE_SIZE,
                                Tanks::TILE_SIZE, Tanks::TILE_SIZE));
            }
            map_sprite.setPosition(
                static_cast<float>(Tanks::MARGIN_LEFT + Tanks::TILE_SIZE * j),
                static_cast<float>(Tanks::MARGIN_TOP + Tanks::TILE_SIZE * i));

            window.draw(map_sprite);
        }
    }
}

// void bullets_control(Tanks::Player &player, std::list<Tanks::Bullet>
// &bullets, double time, sf::RenderWindow &window) {
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
//        player.is_have_shot()) {
//        bullets.emplace_back(Tanks::Bullet(player.get_direction(),
//                                           player.get_object_coordinate()));
//        player.make_shot();
//    }
//
//    for (auto iterator = bullets.begin(); iterator != bullets.end();) {
//        if (!iterator->is_destroyed()) {
//            iterator->update_position(iterator->get_direction(), time);
//            window.draw(iterator->get_bullet_sprite());
////            iterator->check_intersection_with_map();
//            iterator++;
//        }
//    }
//    // needs to be redone
//    bullets.remove_if([&](Tanks::Bullet &bullet) {
//        if (bullet.is_destroyed()) {
//            player.recover_bullet();
//            return true;
//        }
//        return false;
//    });
//}

std::vector<std::vector<std::string>> parse_csv(int level) {
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

void start_game(sf::RenderWindow &window, int level) {
    const std::string tankImageFilename =
        "../images/tanks/player_tanks.png";  // TODO remake initialization

    Tanks::Player player(
        //        "../images/tanks/player_tanks.png",
        sf::Vector2<int>(Tanks::MARGIN_LEFT + Tanks::TILE_SIZE,
                         Tanks::TILE_SIZE * (Tanks::MAP_HEIGHT - 2) +
                             Tanks::MARGIN_TOP * 3));

    Tanks::MovableView playerView(player, tankImageFilename);

    // map
    sf::Image map_image;
    map_image.loadFromFile("../images/map.png");
    sf::Texture map_texture;
    map_texture.loadFromImage(map_image);
    sf::Sprite map_sprite;
    map_sprite.setTexture(map_texture);
    std::vector<std::vector<std::string>> map = parse_csv(level);

    // background
    sf::Image background_image;
    background_image.loadFromFile("../images/environment/background.png");
    sf::Texture background_texture;
    background_texture.loadFromImage(background_image);
    sf::Sprite background_sprite;
    background_sprite.setTexture(background_texture);
    background_sprite.setPosition(0, 0);

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

        make_move(player, time);

        // redraw
        window.clear();
        window.draw(background_sprite);

        draw_map(window, map, map_sprite);

        //        bullets_control(player, bullets, time, window); TODO

        //        player.check_interaction_with_map(); TODO

        playerView.updatePosition();  // TODO make this unnesessary
        window.draw(playerView.getObjectSprite());
        window.display();
        // Hack for better performance
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

}  // namespace Tanks