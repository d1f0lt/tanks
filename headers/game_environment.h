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
    sf::Sprite sprite;
};

struct Lives {
public:
    Lives(const std::string &filename, size_t heartSize, size_t livesCount, size_t mxLivesCount); // NOLINT

    void destroyLastHeart();

    void draw(sf::RenderWindow &window) const;

private:
    sf::Image image;
    sf::Texture texture;
    std::vector<sf::Sprite> sprites;
    size_t livesAmount;
    size_t sizeOfOne;

    void setPosition();
};

struct Environment final {
public:
    explicit Environment(const std::string &path, size_t lifeAmount);

    void destroyLastHeart();

    void draw(sf::RenderWindow &window, bool isPause) const;

    [[nodiscard]] const Menu::Menu &getMenu() const;

private:
    Lives lives;
    mutable Timer timer;
    Menu::Menu menu{};
    sf::Sprite backgroundSprite;
};

}  // namespace Tanks

#endif