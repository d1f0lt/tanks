#include "constants.h"
#include "game.h"

int main() {
    sf::RenderWindow window(
        sf::VideoMode(Tanks::WINDOW_WIDTH, Tanks::WINDOW_HEIGHT),
        "Just Tanks");  // create window
    // Tanks::menu(window); TODO
    Tanks::start_game(window);
    return 0;
}