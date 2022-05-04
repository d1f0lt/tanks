#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "menu.h"
#include "constants.h"

using namespace Tanks::Menu;

TEST_CASE("Test animation for main menu") {
    auto menu = ([]() {
        const static sf::Color textColor{63, 87, 210};
        const static auto menuWidth = static_cast<size_t>(Tanks::WINDOW_WIDTH / 3.4);

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
    })();
    menu.flyAwayToRight();
    for (const auto &item : menu.getItems()) {
        CHECK(item->getPosition().x < 0);
        CHECK(item->getPosition().y == item->getStandardPosition().y);
    }
}