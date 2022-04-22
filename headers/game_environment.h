#ifndef GAME_ENVIRONMENT
#define GAME_ENVIRONMENT

#include "menu.h"
#include "pause.h"

namespace Tanks {

struct Timer final {
public:
    explicit Timer(const std::string &path);

    void drawTimer(sf::RenderWindow &window) const;

    void restart();

    void nextTick();

private:
    int passedTime = 0;
    sf::Clock timer;
    sf::Font font;
    sf::Text time;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
};

struct Environment final {
public:
    explicit Environment(const std::string &path);

    void draw(sf::RenderWindow &window, bool isPause) const;

    const Menu::Menu &getMenu() const;

private:
    mutable Timer timer;
    Menu::Menu menu;
    sf::Sprite background;

    static sf::Sprite initBackground(const std::string &path);

    void addPauseButton(const std::string &path);
};

}

#endif