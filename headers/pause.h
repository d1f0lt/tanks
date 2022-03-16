#ifndef PAUSE_H
#define PAUSE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "constants.h"
#include "controller.h"
#include "menu.h"

namespace Tanks {

struct Pause final {
public:
    Pause();

    void drawPause(sf::RenderWindow &window) const;

    [[nodiscard]] bool isPause() const;

    void checkPause(const sf::Event &event);

    const Menu::Menu &getMenu() const;

    void unpause();

private:
    bool pause = false;
    sf::RectangleShape background;
    const static int pauseWidth = 400;
    // pauseHeight calculate automatically
    const std::string path = "../images/pause/";
    const int buttonsHeight = 80;
    const sf::Color buttonsStandardColor = sf::Color(0, 0, 0);
    const sf::Color buttonsHoverColor = sf::Color(115, 115, 115);
    const std::vector<Menu::ButtonType> buttonTypes = {
        Menu::ButtonType::RESUME, Menu::ButtonType::NEW_GAME,
        Menu::ButtonType::SETTINGS, Menu::ButtonType::EXIT};
    Menu::Menu menu;
};

}  // namespace Tanks

#endif