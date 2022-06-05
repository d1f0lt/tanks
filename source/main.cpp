#include "constants.h"
#include "menu/users_menu.h"
#include "server.h"
#include "sound/background_music.h"

int main(int argc, const char *argv[]) {
    if (argc >= 2 && std::string(argv[1]) == "server") {
        Tanks::createAndRunServer(1, 1);
    }
    static const std::string musicPath = "../sounds/background_music.ogg";
    sf::RenderWindow window(
        sf::VideoMode(Tanks::WINDOW_WIDTH, Tanks::WINDOW_HEIGHT), "Just Tanks",
        sf::Style::Fullscreen);  // create window
    Tanks::Sound::BackgroundMusicHolder backgroundMusic(musicPath);
    backgroundMusic.play(0);
    Tanks::Menu::showUsersMenu(window);
    return 0;
}