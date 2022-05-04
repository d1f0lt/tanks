#include "main_menu.h"
#include "constants.h"
#include "controller.h"
#include "menu.h"
#include "new_game_menu.h"
#include <memory>
#include <string>

namespace Tanks::Menu {

namespace {

sf::Sprite initBackground(const std::string &path) {
    const static std::string backgroundFilename = path + "background.png";
    sf::Image backgroundImage;
    backgroundImage.loadFromFile(backgroundFilename);
    static sf::Texture backgroundTexture;
    backgroundTexture.loadFromImage(backgroundImage);
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);
    return backgroundSprite;
}

Menu initMenu() {
    const static sf::Color textColor{63, 87, 210};
    const static auto menuWidth = static_cast<size_t>(WINDOW_WIDTH / 3.4);

    // title
    const static std::string titleText = "JUST TANKS";
    const static size_t titleCharacterSize = 80;
    InscriptionInfo title{titleText, titleCharacterSize, textColor};

    // inscriptions
    const static std::string inscriptionsText;
    const static size_t inscriptionsCharacterSize = 50;
    InscriptionInfo inscriptions{inscriptionsText, inscriptionsCharacterSize,
                                 textColor};

    // buttons
    const static std::vector<ButtonType> buttonTypes = {
        ButtonType::NEW_GAME, ButtonType::UPGRADE, ButtonType::CREATE_MAP,
        ButtonType::RATING};
    const static size_t buttonsHeight = 100;
    const static sf::Color btnStandardColor(0, 0, 0, 150);
    const static sf::Color btnHoverColor(66, 66, 66, 230);
    std::vector<ButtonWithType> buttons;
    buttons.reserve(buttonTypes.size());
    for (auto type : buttonTypes) {
        buttons.emplace_back(
            ButtonWithType(type, sf::Vector2<float>(menuWidth, buttonsHeight),
                           btnStandardColor, btnHoverColor));
    }

    return Menu(menuWidth, title, inscriptions, buttons);
}
}  // namespace

void showMainMenu(sf::RenderWindow &window) {
    const static std::string imagesPath = "../images/menu/";
    sf::Sprite backgroundSprite(initBackground(imagesPath));

    Menu menu(initMenu());
    menu.addIconToLeftLowerCorner(imagesPath + "exit.png", ButtonType::EXIT);
    menu.addIconToLeftUpCorner(imagesPath + "gear.png", ButtonType::SETTINGS);

    while (window.isOpen()) {
        const auto res = menu.showMenu(window, backgroundSprite);
        switch (res->getType()) {
            case ButtonType::NEW_GAME: {
                menu.flyAwayToLeft(window, backgroundSprite);
                showNewGameMenu(window, backgroundSprite);
                menu.flyOutFromLeft(window, backgroundSprite);
                break;
            }
            case ButtonType::EXIT:
                window.close();
            default:
                break;
        }
    }
}

}  // namespace Tanks::Menu