#include "main_menu.h"
#include <memory>
#include <string>
#include "constants.h"
#include "controller.h"
#include "game.h"
#include "menu.h"

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
    const static sf::Color textColor{0, 0, 255};
    const static int menuWidth = static_cast<int>(WINDOW_WIDTH / 3.4);

    // title
    const static std::string titleText = "JUST TANKS";
    const static int titleCharacterSize = 80;
    InscriptionInfo title{titleText, titleCharacterSize, textColor};

    // inscriptions
    const static std::string inscriptionsText;
    const static int inscriptionsCharacterSize = 50;
    InscriptionInfo inscriptions{inscriptionsText, inscriptionsCharacterSize, textColor};

    // buttons
    const static std::vector<ButtonType> buttonTypes = {ButtonType::NEW_GAME, ButtonType::UPGRADE, ButtonType::CREATE_MAP, ButtonType::RATING};
    const static int buttonsHeight = 100;
    const static sf::Color btnStandardColor(0, 0, 0, 150);
    const static sf::Color btnHoverColor(66, 66, 66, 230);
    std::vector<ButtonInfo> buttons;
    buttons.reserve(buttonTypes.size());
    for (auto type : buttonTypes) {
        buttons.emplace_back(ButtonInfo{type, buttonsHeight, btnStandardColor, btnHoverColor});
    }

    return Menu(menuWidth, title, inscriptions, buttons);
}

void addExitButton(Menu &menu, const std::string &path) {
    sf::Image exitImage;
    exitImage.loadFromFile(path + "exit.png");
    const int margin = 5;
    sf::Vector2<float> coordinates(
        margin, static_cast<float>(WINDOW_HEIGHT - exitImage.getSize().y - 3*margin));
    sf::Vector2<float> rectangleSize(
        static_cast<float>(exitImage.getSize().x + 2*margin),
        static_cast<float>(exitImage.getSize().y + 2*margin));
    auto picture = std::make_unique<MenuPicture>(path + "exit.png", coordinates);
    menu.addMenuItem(std::make_unique<MenuButton>(
        std::move(picture), coordinates, rectangleSize, sf::Color(0, 0, 0, 0),
        sf::Color(128, 128, 128, 128), ButtonType::EXIT));
}

void activity(ButtonType type, sf::RenderWindow &window) {  // TODO rename
    switch (type) {
        case ButtonType::NEW_GAME: {
            auto res = startGame(window);
            while (res != std::nullopt) {
                res = startGame(window);  // TODO make others
            }
            break;
        }
            /*
        case ButtonType::CREATE_MAP:
            break;
        case ButtonType::SETTINGS:
            break;
        case ButtonType::STAT:
            break;
             */
        case ButtonType::EXIT:
            window.close();
        default:
            break;
    }
}
}  // namespace

void menu(sf::RenderWindow &window) {
    const static std::string imagesPath = "../images/menu/";
    sf::Sprite backgroundSprite(initBackground(imagesPath));

    Menu menu(initMenu());
    addExitButton(menu, imagesPath);

    while (window.isOpen()) {
        // catch event
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (const auto res = Tanks::MenuController::control(menu, window);
            res != std::nullopt) {
            activity(res.value(), window);
        }

        // redraw
        window.clear();
        window.draw(backgroundSprite);
        menu.draw(window);
        window.display();
    }
}

}  // namespace Tanks::Menu