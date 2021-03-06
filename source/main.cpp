#include "constants.h"
#include "menu/users_menu.h"
#include "server.h"
#include "sound/background_music.h"

int main(int argc, const char *argv[]) {
    if (argc >= 3 && std::string(argv[1]) == "server") {
        int level = std::stoi(argv[2]);
        if (level <= 0 || level >= 4) {
            return 123;
        }
        int players = 2;
        if (argc >= 4) {
            players = std::stoi(argv[3]);
            if (players <= 0) {
                return 123;
            }
        }
        int bots = 10;
        if (argc >= 5) {
            bots = std::stoi(argv[4]);
            if (bots < 0) {
                return 123;
            }
        }
        int bonuses = 2;
        if (argc >= 6) {
            bonuses = std::stoi(argv[5]);
            if (bonuses < 0) {
                return 123;
            }
        }
        Tanks::createAndRunServer(level, players, bots, bonuses);
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