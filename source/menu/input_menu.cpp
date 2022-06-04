#include "menu/input_menu.h"
#include <cassert>
#include "constants.h"
#include "menu/menu.h"
#include "menu/menu_controller.h"
#include "menu/textbox.h"

namespace Tanks::Menu {

Menu initMenu(const std::string &titleText) {
    const static sf::Color textColor{63, 87, 210};
    const static int limit = 18;

    Menu menu{};

    const static float marginFromTitle = 80;
    const static size_t titleCharacterSize = 80;
    InscriptionInfo titleInfo{titleText, titleCharacterSize, textColor};
    auto title =
        std::make_unique<MenuInscription>(titleInfo, sf::Vector2<float>{0, 0});

    const static size_t textSize = 50;

    const float menuWidth = static_cast<float>(WINDOW_WIDTH) / 3;
    const static sf::Vector2<float> textboxSize{menuWidth, 80};

    sf::Vector2<float> curCoordinates{
        (WINDOW_WIDTH - textboxSize.x) / 2,
        (WINDOW_HEIGHT - title->getSize().y - marginFromTitle - textboxSize.y) /
            2};
    title->setPosition(curCoordinates);
    curCoordinates.y += title->getSize().y + marginFromTitle;

    InscriptionInfo info{"", textSize, textColor};

    auto textbox = std::make_unique<TextBox>(
        limit, info, textboxSize, curCoordinates, sf::Color(0, 0, 0));

    title->centralizeByWidth({curCoordinates.x, curCoordinates.x + textbox->getSize().x});
    title->updateStandardPosition();
    menu.addMenuItem(std::move(title));
    menu.addMenuItem(std::move(textbox));

    return menu;
}

std::optional<std::string> showInputMenu(sf::RenderWindow &window,
                                         const sf::Sprite &background,
                                         const std::string &title) {
    const static std::string imagesPath = "../images/menu/";

    auto menu(initMenu(title));
    menu.addIconToLeftUpCorner(imagesPath + "return.png", ButtonType::RETURN);

    menu.flyAwayToRight();
    menu.flyOutFromRight(window, background);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                auto ans = MenuController::textEntered(menu, event);
                if (ans != std::nullopt) {
                    return ans.value();
                }
            }
        }

        if (const auto res =
                Tanks::MenuController::control(menu, window, event);
            res != std::nullopt) {
            assert(res.value()->getType() == ButtonType::RETURN);
            menu.flyAwayToRight(window, background);
            return std::nullopt;
        }

        window.clear();
        window.draw(background);
        menu.draw(window);
        window.display();
    }
    return std::nullopt;
}
}  // namespace Tanks::Menu
