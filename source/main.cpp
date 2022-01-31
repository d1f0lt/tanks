#include "../headers/constants.h"
#include "../headers/tank.h"
#include "../headers/player.h"
#include "../headers/map.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(Tanks::WINDOW_WIDTH, Tanks::WINDOW_HEIGHT),
                            "Just Tanks"); // create window

    Tanks::Player player("../images/tanks/player_tank_up.png", {25, Tanks::WINDOW_HEIGHT - 32 - 48});

    // temp
    sf::Image map_image;
    map_image.loadFromFile("../images/map.png");
    sf::Texture map_texture;
    map_texture.loadFromImage(map_image);
    sf::Sprite map_sprite;
    map_sprite.setTexture(map_texture);
    //

    sf::Clock clock;

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

        // Movement
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

        // redraw
        window.clear();

        // temp
        for (int i = 0; i < HEIGHT_MAP; ++i) {
            for (int j = 0; j < WIDTH_MAP; ++j) {
                if (tile_map[i][j] == ' ')
                    map_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
                else if (tile_map[i][j] == 's')
                    map_sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
                else if (tile_map[i][j] == '0')
                    map_sprite.setTextureRect(sf::IntRect(64, 0, 32, 32));

                map_sprite.setPosition(static_cast<float>(32 * j), static_cast<float>(32 * i));

                window.draw(map_sprite);
            }
        }
        //
        player.interaction_with_map();
        window.draw(player.get_tank_sprite());
        window.display();
    }

    return 0;
}