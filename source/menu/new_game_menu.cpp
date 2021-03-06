#include "menu/new_game_menu.h"
#include <cassert>
#include <thread>
#include "menu/levels_menu.h"
#include "menu/multiplayer_menu.h"
#include "sound/background_music.h"

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
    const static size_t inscriptionsCharacterSize = 50;

    // buttons
    const static std::vector<ButtonType> buttonTypes = {
        ButtonType::SINGLE_PLAYER, ButtonType::MULTIPLAYER};
    const static size_t buttonsHeight = 100;
    const static sf::Color btnStandardColor(0, 0, 0, 150);
    const static sf::Color btnHoverColor(66, 66, 66, 230);
    std::vector<ButtonWithType> buttons;
    std::vector<std::unique_ptr<MenuItem>> inscriptions;
    buttons.reserve(buttonTypes.size());
    inscriptions.reserve(buttonTypes.size());
    for (auto type : buttonTypes) {
        buttons.emplace_back(
            ButtonWithType{type, sf::Vector2<float>(menuWidth, buttonsHeight),
                           btnStandardColor, btnHoverColor});
        InscriptionInfo info{convertButtonTypeToString(type),
                             inscriptionsCharacterSize, textColor};
        auto item =
            std::make_unique<MenuInscription>(info, sf::Vector2<float>{0, 0});
        inscriptions.emplace_back(std::move(item));
    }

    return Menu(menuWidth, title, inscriptions, buttons);
}
}  // namespace

void showNewGameMenu(sf::RenderWindow &window,
                     const sf::Sprite &backgroundSprite,
                     PlayerInfo &info,
                     Sound::BackgroundMusicHolder &backgroundMusicHolder) {
    const static std::string imagesPath = "../images/menu/";
    Menu menu(initMenu());
    menu.addIconToLeftUpCorner(imagesPath + "return.png", ButtonType::RETURN);

    menu.flyAwayToRight();
    menu.flyOutFromRight(window, backgroundSprite);

    while (window.isOpen()) {
        const auto *const res = menu.showMenu(window, backgroundSprite);
        switch (res->getType()) {
            case ButtonType::SINGLE_PLAYER: {
                menu.flyAwayToLeft(window, backgroundSprite);
                auto ans = showLevelsMenu(window, backgroundSprite, info, backgroundMusicHolder);
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
                menu.flyAwayToLeft(window, backgroundSprite);
                showMultiplayerMenu(window, backgroundSprite, info, backgroundMusicHolder);
                menu.flyOutFromLeft(window, backgroundSprite);
                break;

            case ButtonType::RETURN:
                menu.flyAwayToRight(window, backgroundSprite);
                return;

            default:
                assert(false);
        }
    }
}

}  // namespace Tanks::Menu