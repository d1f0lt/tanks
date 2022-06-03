#include "menu/levels_menu.h"
#include <cassert>
#include "constants.h"
#include "game.h"

namespace Tanks::Menu {

namespace {

Menu initMenu(const std::string &imagesPath) {
    const static sf::Color textColor{63, 87, 210};
    const static size_t menuWidth = static_cast<int>(WINDOW_WIDTH / 1.5);
    const static size_t quantityPerLine = 4;
    const static size_t quantityElement = 7;
    const static sf::Color standardColor(0, 0, 0, 150);

    // title
    const static std::string titleText = "SELECT LEVEL";
    const static size_t titleCharacterSize = 76;
    const static InscriptionInfo title{titleText, titleCharacterSize,
                                       textColor};

    // inscriptions
    std::vector<InscriptionInfo> inscriptions;
    inscriptions.reserve(quantityElement);
    const static int characterSize = 32;
    for (size_t i = 1; i <= quantityElement; ++i) {
        inscriptions.emplace_back(
            InscriptionInfo{std::to_string(i), characterSize, textColor});
    }

    const static sf::Color btnStandardColor(0, 0, 0, 150);
    const static sf::Color btnHoverColor(66, 66, 66, 230);
    Button btnInfo{sf::Vector2<float>(0, 0), btnStandardColor, btnHoverColor};
    return Menu(menuWidth, title, inscriptions, imagesPath, quantityPerLine,
                btnInfo);
}

}  // namespace

ButtonType showLevelsMenu(sf::RenderWindow &window,
                          const sf::Sprite &backgroundSprite,
                          [[maybe_unused]] PlayerInfo &info) {
    const static std::string imagesPath = "../images/menu/";
    Menu menu(initMenu(imagesPath + "levels/"));
    menu.addIconToLeftUpCorner(imagesPath + "return.png", ButtonType::RETURN);

    menu.flyAwayToRight();
    menu.flyOutFromRight(window, backgroundSprite);

    while (window.isOpen()) {
        const auto *res = menu.showMenu(window, backgroundSprite);
        switch (res->getType()) {
            case ButtonType::LEVEL: {
                const auto *item =
                    dynamic_cast<const MenuPictureWithDescription *>(res);
                size_t level = std::stoi(item->getDescription());
                if (level > LEVELS_COUNT) {
                    continue;
                }
                auto ans =
                    startGame(window, info, std::stoi(item->getDescription()));
                assert(ans != std::nullopt);
                switch (ans.value()) {
                    case ButtonType::EXIT:
                        return ans.value();
                    case ButtonType::NEW_GAME:
                        continue;
                    default:
                        assert(false);
                }
            }
            case ButtonType::RETURN:
                menu.flyAwayToRight(window, backgroundSprite);
                return ButtonType::RETURN;
            default:
                assert(false);
        }
    }
    return ButtonType::EXIT;
}

}  // namespace Tanks::Menu