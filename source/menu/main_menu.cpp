#include "menu/main_menu.h"
#include <memory>
#include <string>
#include "constants.h"
#include "menu.h"
#include "menu/new_game_menu.h"
#include "menu/upgrade_menu.h"
#include "menu/settings_menu.h"
#include "menu/rating_menu.h"
#include <cassert>

namespace Tanks::Menu {

namespace {

Menu initMenu(const std::string &imagesPath, PlayerGeneral &playersInfo) {
    const static sf::Color textColor{63, 87, 210};
    const static auto menuWidth = static_cast<size_t>(WINDOW_WIDTH / 3.4);

    // title
    const static std::string titleText = "JUST TANKS";
    const static size_t titleCharacterSize = 80;
    InscriptionInfo title{titleText, titleCharacterSize, textColor};

    const static size_t inscriptionsCharacterSize = 50;

    // buttons
    const static std::vector<ButtonType> buttonTypes = {
        ButtonType::NEW_GAME, ButtonType::UPGRADE, ButtonType::RATING,
        ButtonType::SETTINGS};
    const static size_t buttonsHeight = 100;
    const static sf::Color btnStandardColor(0, 0, 0, 150);
    const static sf::Color btnHoverColor(66, 66, 66, 230);
    std::vector<ButtonWithType> buttons;
    std::vector<std::unique_ptr<MenuItem>> inscriptions;
    buttons.reserve(buttonTypes.size());
    inscriptions.reserve(buttonTypes.size());
    for (auto type : buttonTypes) {
        buttons.emplace_back(
            ButtonWithType(type, sf::Vector2<float>(menuWidth, buttonsHeight),
                           btnStandardColor, btnHoverColor));
        InscriptionInfo info{convertButtonTypeToString(type),
                             inscriptionsCharacterSize, textColor};
        auto item =
            std::make_unique<MenuInscription>(info, sf::Vector2<float>{0, 0});
        inscriptions.emplace_back(std::move(item));
    }

    Menu menu(menuWidth, title, inscriptions, buttons);

    menu.addIconToLeftLowerCorner(imagesPath + "exit.png", ButtonType::EXIT);
    menu.addIconToLeftUpCorner(imagesPath + "users.png", ButtonType::USERS);
    menu.addPlayerInfo(playersInfo);

    return menu;
}

}  // namespace

void showMainMenu(sf::RenderWindow &window,
                  const sf::Sprite &backgroundSprite,
                  PlayerInfo &info) {
    const static std::string imagesPath = "../images/menu/";

    Menu menu(initMenu(imagesPath, info.general));

    menu.flyAwayToRight();
    menu.flyOutFromRight(window, backgroundSprite);

    while (window.isOpen()) {
        const auto *const res = menu.showMenu(window, backgroundSprite);
        switch (res->getType()) {
            case ButtonType::NEW_GAME: {
                menu.flyAwayToLeft(window, backgroundSprite);
                showNewGameMenu(window, backgroundSprite, info);
                menu.flyOutFromLeft(window, backgroundSprite);
            } break;
            case ButtonType::EXIT:
                window.close();
                break;
            case ButtonType::USERS:
                menu.flyAwayToRight(window, backgroundSprite);
                return;
            case ButtonType::UPGRADE:
                menu.flyAwayToLeft(window, backgroundSprite);
                showUpgradeMenu(window, backgroundSprite, info);
                menu = initMenu(imagesPath, info.general);
                menu.flyAwayToLeft();
                menu.flyOutFromLeft(window, backgroundSprite);
                break;
            case ButtonType::SETTINGS:
                menu.flyAwayToLeft(window, backgroundSprite);
                showSettingsMenu(window, backgroundSprite, info);
                menu = initMenu(imagesPath, info.general);
                menu.flyAwayToLeft();
                menu.flyOutFromLeft(window, backgroundSprite);
                break;
            case ButtonType::RATING:
                menu.flyAwayToLeft(window, backgroundSprite);
                showRatingMenu(window, backgroundSprite, info);
                menu.flyOutFromLeft(window, backgroundSprite);
                break;
            default:
                assert(false);
        }
    }
}

}  // namespace Tanks::Menu