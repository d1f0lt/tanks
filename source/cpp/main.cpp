#include "../headers/constants.h"
#include "../headers/tank.h"
#include "../headers/player.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(Tanks::WINDOW_WIDTH, Tanks::WINDOW_HEIGHT),
                            "Just Tanks"); // create window

    Tanks::Player player("../images/hero.png", {-10, Tanks::WINDOW_HEIGHT - 50});

    double current_frame = 0; // can be from 1 to 3
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
            player.update_position(
                Tanks::Direction::LEFT, current_frame, time,
                {Tanks::ONE_TILE_WIDTH, 0, Tanks::ONE_TILE_WIDTH, Tanks::ONE_TILE_HEIGHT});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
                   sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player.update_position(Tanks::Direction::RIGHT, current_frame, time,
                                   {Tanks::ONE_TILE_WIDTH, Tanks::ONE_TILE_HEIGHT * 2,
                                    Tanks::ONE_TILE_WIDTH, Tanks::ONE_TILE_HEIGHT});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
                   sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.update_position(
                Tanks::Direction::DOWN, current_frame, time,
                {Tanks::ONE_TILE_WIDTH, 0, Tanks::ONE_TILE_WIDTH, Tanks::ONE_TILE_HEIGHT});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
                   sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player.update_position(Tanks::Direction::UP, current_frame, time,
                                   {Tanks::ONE_TILE_WIDTH, Tanks::ONE_TILE_HEIGHT * 3,
                                    Tanks::ONE_TILE_WIDTH, Tanks::ONE_TILE_HEIGHT});
        }

        // redraw
        window.clear();
        window.draw(player.get_tank_sprite());
        window.display();
    }

    return 0;
}