#include "constants.h"
#include "menu/main_menu.h"

int main() {
    sf::RenderWindow window(
        sf::VideoMode(Tanks::WINDOW_WIDTH, Tanks::WINDOW_HEIGHT), "Just Tanks",
        sf::Style::Fullscreen);  // create window
    Tanks::Menu::showMainMenu(window);
    return 0;
}