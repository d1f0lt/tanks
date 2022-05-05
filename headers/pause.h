#ifndef PAUSE_H
#define PAUSE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "menu.h"

namespace Tanks {

struct Pause final {
public:
    Pause();

    void makePause();

    void drawPause(sf::RenderWindow &window) const;

    [[nodiscard]] bool isPause() const;

    void checkPause(const sf::Event &event);

    const Menu::Menu &getMenu() const;

    void unpause();

private:
    bool pause = false;
    sf::RectangleShape background;
    Menu::Menu menu;
};

}  // namespace Tanks

#endif