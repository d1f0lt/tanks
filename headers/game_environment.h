#ifndef GAME_ENVIRONMENT
#define GAME_ENVIRONMENT

#include "menu/menu.h"
#include "pause.h"

namespace Tanks {

struct Timer final {
public:
    explicit Timer(const std::string &filename);

    void draw(sf::RenderWindow &window) const;

    void restart();

    void nextTick();

private:
    int passedTime = 0;
    sf::Clock timer;
    sf::Font font;
    sf::Text time;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;  // TODO make menuItem for this
};

struct Environment final {
public:
    explicit Environment(const std::string &path);

    void draw(sf::RenderWindow &window, bool isPause) const;

    [[nodiscard]] const Menu::Menu &getMenu() const;

private:
    mutable Timer timer;
    Menu::Menu menu{};
    sf::Sprite backgroundSprite;
};

}  // namespace Tanks

#endif