#include "new_game_menu.h"
#include <cassert>
#include <thread>
#include "controller.h"
#include "game.h"
#include "levels_menu.h"

namespace Tanks::Menu {

namespace {

Menu initMenu() {
    const static sf::Color textColor{63, 87, 210};
    const static auto menuWidth = static_cast<size_t>(WINDOW_WIDTH / 3.4);

    // title
    const static std::string titleText = "SELECT MODE";
    const static size_t titleCharacterSize = 76;
    InscriptionInfo title{titleText, titleCharacterSize, textColor};

    // inscriptions
    const static std::string inscriptionsText;
    const static size_t inscriptionsCharacterSize = 50;
    InscriptionInfo inscriptions{inscriptionsText, inscriptionsCharacterSize,
                                 textColor};

    // buttons
    const static std::vector<ButtonType> buttonTypes = {
        ButtonType::SINGLE_PLAYER, ButtonType::MULTIPLAYER};
    const static size_t buttonsHeight = 100;
    const static sf::Color btnStandardColor(0, 0, 0, 150);
    const static sf::Color btnHoverColor(66, 66, 66, 230);
    std::vector<ButtonWithType> buttons;
    buttons.reserve(buttonTypes.size());
    for (auto type : buttonTypes) {
        buttons.emplace_back(
            ButtonWithType{type, sf::Vector2<float>(menuWidth, buttonsHeight),
                           btnStandardColor, btnHoverColor});
    }

    return Menu(menuWidth, title, inscriptions, buttons);
}
}  // namespace

void showNewGameMenu(sf::RenderWindow &window,
                     const sf::Sprite &backgroundSprite) {
    const static std::string imagesPath = "../images/menu/";
    Menu menu(initMenu());
    menu.addIconToLeftUpCorner(imagesPath + "return.png", ButtonType::RETURN);

    menu.flyAwayToRight();
    menu.flyOutFromRight(window, backgroundSprite);

    while (window.isOpen()) {
        // catch event
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (const auto res =
                Tanks::MenuController::control(menu, window, event);
            res != std::nullopt) {
            switch (res.value()->getType()) {
                case ButtonType::SINGLE_PLAYER: {
                    menu.flyAwayToLeft(window, backgroundSprite);
                    auto ans = showLevelsMenu(window, backgroundSprite);
                    switch (ans) {
                        case ButtonType::RETURN:
                            menu.flyOutFromLeft(window, backgroundSprite);
                            continue;
                        case ButtonType::EXIT:
                            return;
                        default:
                            assert(false);
                    }
                }
                case ButtonType::MULTIPLAYER:

                case ButtonType::RETURN:
                    menu.flyAwayToRight(window, backgroundSprite);
                    return;

                default:
                    assert(false);
            }
        }

        // redraw
        window.clear();
        window.draw(backgroundSprite);
        menu.draw(window);
        window.display();
    }
}

}  // namespace Tanks::Menu