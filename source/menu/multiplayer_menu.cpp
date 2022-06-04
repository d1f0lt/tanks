#include "menu/multiplayer_menu.h"
#include <cassert>
#include <thread>
#include "game.h"
#include "menu/input_menu.h"
#include "menu/levels_menu.h"
#include <boost/system/system_error.hpp>

namespace Tanks::Menu {

namespace {

Menu initMenu() {
    const static sf::Color textColor{63, 87, 210};
    const static auto menuWidth = static_cast<size_t>(WINDOW_WIDTH / 2.5);

    // title
    const static std::string titleText;
    const static size_t titleCharacterSize = 76;
    InscriptionInfo title{titleText, titleCharacterSize, textColor};

    // inscriptions
    const static size_t inscriptionsCharacterSize = 50;

    // buttons
    const static std::vector<ButtonType> buttonTypes = {
        ButtonType::CREATE_SERVER, ButtonType::CONNECT};
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

void showMultiplayerMenu(sf::RenderWindow &window,
                         const sf::Sprite &backgroundSprite,
                         PlayerInfo &info) {
    const static std::string imagesPath = "../images/menu/";
    Menu menu(initMenu());
    menu.addIconToLeftUpCorner(imagesPath + "return.png", ButtonType::RETURN);

    menu.flyAwayToRight();
    menu.flyOutFromRight(window, backgroundSprite);

    while (window.isOpen()) {
        const auto *const res = menu.showMenu(window, backgroundSprite);
        switch (res->getType()) {
            case ButtonType::CREATE_SERVER: {
                menu.flyAwayToLeft(window, backgroundSprite);
                auto ans = showLevelsMenu(window, backgroundSprite, info);
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
            case ButtonType::CONNECT: {
                menu.flyAwayToLeft(window, backgroundSprite);
                auto ip = showInputMenu(window, backgroundSprite, "INPUT IP", // NOLINT
                                        "192.168.0.1", FlyDirection::LEFT);
                if (ip != std::nullopt) {
                    auto port = showInputMenu(window, backgroundSprite,
                                              "INPUT PORT", "80");
                    if (port != std::nullopt) {
                        try {
                            auto address =
                                std::make_pair(ip.value(), port.value());
                            std::optional<std::pair<std::string, std::string>>
                                opt(address);
                            startGame(window, -1, info.skills, opt,
                                      MAX_PLAYERS_AMOUNT);
                        } catch (const boost::system::system_error &err) {
                            menu.flyOutFromLeft(window, backgroundSprite);
                        }
                    }
                }
                menu.flyOutFromLeft(window, backgroundSprite);
            } break;

            case ButtonType::RETURN:
                menu.flyAwayToRight(window, backgroundSprite);
                return;

            default:
                assert(false);
        }
    }
}

}  // namespace Tanks::Menu