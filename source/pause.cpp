#include "pause.h"
#include <cassert>
#include "controller.h"

namespace Tanks {

Pause::Pause()
    : background(sf::Vector2<float>(WINDOW_WIDTH, WINDOW_HEIGHT)),
      menu(path,
           pauseWidth,
           buttonTypes,
           buttonsHeight,
           buttonsStandardColor,
           buttonsHoverColor) {
    background.setPosition(0, 0);
    background.setFillColor(sf::Color(0, 0, 0, 180));
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

void Pause::unpause() {
    assert(pause);
    pause = false;
}

}  // namespace Tanks