#include <iostream>
#include <list>
#include "../headers/bullet.h"
#include "../headers/constants.h"
#include "../headers/map.h"
#include "../headers/player.h"

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



int main() {
    sf::RenderWindow window(
        sf::VideoMode(Tanks::WINDOW_WIDTH, Tanks::WINDOW_HEIGHT),
        "Just Tanks");  // create window

    Tanks::Player player("../images/tanks/player_tank_up.png",
                         {25 + Tanks::ONE_TILE_WIDTH * Tanks::SHIFT,
                          Tanks::WINDOW_HEIGHT - Tanks::ONE_TILE_HEIGHT - 48});

    // temp
    sf::Image map_image;
    map_image.loadFromFile("../images/map.png");
    sf::Texture map_texture;
    map_texture.loadFromImage(map_image);
    sf::Sprite map_sprite;
    map_sprite.setTexture(map_texture);
    //

    // background
    sf::Image background_image;
    background_image.loadFromFile("../images/environment/background.png");
    sf::Texture background_texture;
    background_texture.loadFromImage(background_image);
    sf::Sprite background_sprite;
    background_sprite.setTexture(background_texture);
    background_sprite.setPosition(0, 0);

    sf::Clock clock;

    std::list<Tanks::Bullet> bullets;

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

        // temp
        for (int i = 0; i < HEIGHT_MAP; ++i) {
            for (int j = Tanks::SHIFT; j < WIDTH_MAP + Tanks::SHIFT; ++j) {
                if (tile_map[i][j - Tanks::SHIFT] == ' ')
                    map_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
                else if (tile_map[i][j - Tanks::SHIFT] == '0')
                    map_sprite.setTextureRect(sf::IntRect(64, 0, 32, 32));

                map_sprite.setPosition(static_cast<float>(32 * j),
                                       static_cast<float>(32 * i));

                window.draw(map_sprite);
            }
        }
        //

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
            player.is_have_shot()) {
            bullets.emplace_back(Tanks::Bullet(player.get_direction(),
                                               player.get_object_coordinate()));
            player.make_shot();
        }

        for (auto iterator = bullets.begin(); iterator != bullets.end();) {
            if (!iterator->is_destroyed()) {
                iterator->update_position(iterator->get_direction(), time);
                window.draw(iterator->get_bullet_sprite());
                iterator->check_intersection_with_map();
                iterator++;
            }
        }
        // needs to be redone
        bullets.remove_if([&](Tanks::Bullet &bullet) {
            if (bullet.is_destroyed()) {
                player.recover_bullet();
                return true;
            }
            return false;
        });

        player.check_interaction_with_map();
        window.draw(player.get_tank_sprite());
        window.display();
    }

    return 0;
}