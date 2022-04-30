#include "pause.h"
#include <cassert>
#include "controller.h"

namespace Tanks {

namespace {

Menu::Menu initMenu() {
    const static int pauseWidth = 400;

    const static sf::Color textColor{255, 255, 255};

    // title
    const static std::string titleText = "PAUSED";
    const static int titleCharacterSize = 70;
    Menu::InscriptionInfo title{titleText, titleCharacterSize, textColor};

    // inscriptions
    const static std::string inscriptionsText;
    const static int inscriptionsCharacterSize = 36;
    Menu::InscriptionInfo inscriptions{inscriptionsText,
                                       inscriptionsCharacterSize, textColor};

    // buttons
    const std::vector<Menu::ButtonType> buttonTypes = {
        Menu::ButtonType::RESUME, Menu::ButtonType::NEW_GAME,
        Menu::ButtonType::SETTINGS, Menu::ButtonType::EXIT};
    const int buttonsHeight = 80;
    const sf::Color btnStandardColor = sf::Color(0, 0, 0);
    const sf::Color btnHoverColor = sf::Color(115, 115, 115);
    std::vector<Menu::ButtonWithType> buttons;
    buttons.reserve(buttonTypes.size());
    for (auto type : buttonTypes) {
        buttons.emplace_back(Menu::ButtonWithType{
            type, sf::Vector2<float>(pauseWidth, buttonsHeight),
            btnStandardColor, btnHoverColor});
    }

    return Menu::Menu(pauseWidth, title, inscriptions, buttons);
}

}  // namespace

Pause::Pause()
    : background(sf::Vector2<float>(WINDOW_WIDTH, WINDOW_HEIGHT)),
      menu(initMenu()) {
    background.setPosition(0, 0);
    background.setFillColor(sf::Color(0, 0, 0, 180));
}

void Pause::makePause() {
    assert(!pause);
    pause = true;
}

void Pause::drawPause(sf::RenderWindow &window) const {
    window.draw(background);
    menu.draw(window);
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