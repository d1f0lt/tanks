#include "constants.h"
#include "menu/users_menu.h"

int main() {
    sf::RenderWindow window(
        sf::VideoMode(Tanks::WINDOW_WIDTH, Tanks::WINDOW_HEIGHT), "Just Tanks",
        sf::Style::Fullscreen);  // create window
    Tanks::Menu::showUsersMenu(window);
    return 0;
}