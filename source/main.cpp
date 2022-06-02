#include "constants.h"
#include "menu/users_menu.h"
#include "sound/background_music.h"

int main() {
    sf::RenderWindow window(
        sf::VideoMode(Tanks::WINDOW_WIDTH, Tanks::WINDOW_HEIGHT), "Just Tanks",
        sf::Style::Fullscreen);  // create window
    Tanks::Sound::BackgroundMusicHolder backgroundMusic(musicPath);
    backgroundMusic.play(100);
    Tanks::Menu::showUsersMenu(window);
    return 0;
}