#include "pause.h"
#include <cassert>
#include "controller.h"

namespace Tanks {

Pause::Pause()
    : background(sf::Vector2<float>(WINDOW_WIDTH, WINDOW_HEIGHT)), menu() {
    background.setPosition(0, 0);
    background.setFillColor(sf::Color(0, 0, 0, 180));

    const std::string path = "../images/pause/";
    const int buttonsHeight = 80;
    const sf::Color buttonsStandardColor = sf::Color(0, 0, 0);
    const sf::Color buttonsHoverColor = sf::Color(115, 115, 115);
    const std::vector<Menu::ButtonType> buttonTypes = {
        Menu::ButtonType::RESUME, Menu::ButtonType::NEW_GAME,
        Menu::ButtonType::SETTINGS, Menu::ButtonType::EXIT};
    menu = Menu::Menu(path, pauseWidth, buttonTypes, buttonsHeight,
                      buttonsStandardColor, buttonsHoverColor);
}

void Pause::makePause() {
    assert(!pause);
    pause = true;
}

void Pause::drawPause(sf::RenderWindow &window) const {
    window.draw(background);
    menu.drawMenu(window);
}

void Pause::checkPause(const sf::Event &event) {
    pause ^= GameController::isEscReleased(event);
}

bool Pause::isPause() const {
    return pause;
}

const Menu::Menu &Pause::getMenu() const {
    return menu;
}

void Pause::makeUnPause() {
    assert(pause);
    pause = false;
}

}  // namespace Tanks