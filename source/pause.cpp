#include "pause.h"
#include <cassert>
#include "constants.h"
#include "game_controller.h"

namespace Tanks {

namespace {

Menu::Menu initMenu() {
    const static size_t pauseWidth = 400;

    const static sf::Color textColor{255, 255, 255};

    // title
    const static std::string titleText = "PAUSED";
    const static size_t titleCharacterSize = 70;
    Menu::InscriptionInfo title{titleText, titleCharacterSize, textColor};

    // inscriptions
    const static size_t inscriptionsCharacterSize = 36;

    // buttons
    const std::vector<Menu::ButtonType> buttonTypes = {
        Menu::ButtonType::RESUME, Menu::ButtonType::NEW_GAME,
        Menu::ButtonType::SETTINGS, Menu::ButtonType::EXIT};
    const size_t buttonsHeight = 80;
    const sf::Color btnStandardColor = sf::Color(0, 0, 0);
    const sf::Color btnHoverColor = sf::Color(115, 115, 115);
    std::vector<Menu::ButtonWithType> buttons;
    std::vector<std::unique_ptr<Menu::MenuItem>> inscriptions;
    buttons.reserve(buttonTypes.size());
    inscriptions.reserve(buttonTypes.size());
    for (auto type : buttonTypes) {
        buttons.emplace_back(Menu::ButtonWithType{
            type, sf::Vector2<float>(pauseWidth, buttonsHeight),
            btnStandardColor, btnHoverColor});
        Menu::InscriptionInfo info{convertButtonTypeToString(type),
                                   inscriptionsCharacterSize, textColor};
        auto item = std::make_unique<Menu::MenuInscription>(
            info, sf::Vector2<float>{0, 0});
        inscriptions.emplace_back(std::move(item));
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
    pause ^= static_cast<int>(GameController::isEscReleased(event));
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