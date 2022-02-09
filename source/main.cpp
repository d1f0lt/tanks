#include "constants.h"
#include "game.h"

int main() {
    sf::RenderWindow window(
        sf::VideoMode(Tanks::WINDOW_WIDTH, Tanks::WINDOW_HEIGHT),
        "Just Tanks");  // create window
    // menu(window); TODO
    Tanks::startGame(window);
    return 0;
}