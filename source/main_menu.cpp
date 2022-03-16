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

Menu initMenu(const std::string &path) {
    const static std::vector<ButtonType> buttonTypes = {
        ButtonType::NEW_GAME, ButtonType::CREATE_MAP, ButtonType::STAT,
        ButtonType::SETTINGS};
    const static int menuWidth = static_cast<int>(WINDOW_WIDTH / 3.4);
    const static int buttonsHeight = 100;
    const static sf::Color buttonsStandardColor(0, 0, 0, 150);
    const static sf::Color buttonsHoverColor(66, 66, 66, 230);
    return Menu(path, menuWidth, buttonTypes, buttonsHeight,
                buttonsStandardColor, buttonsHoverColor);
}

void addExitButton(Menu &menu, const std::string &path) {
    sf::Image exitImage;
    exitImage.loadFromFile(path + "exit.png");
    const int margin = 10;
    sf::Vector2<float> coordinates(
        0, static_cast<float>(WINDOW_HEIGHT - exitImage.getSize().y - margin));
    sf::Vector2<float> rectangleSize(
        static_cast<float>(exitImage.getSize().x + margin),
        static_cast<float>(exitImage.getSize().y + margin));
    menu.addMenuItem(std::make_unique<MenuButton>(
        path + "exit.png", coordinates, rectangleSize, sf::Color(0, 0, 0, 0),
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

    Menu menu(initMenu(imagesPath));
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
        menu.drawMenu(window);
        window.display();
    }
}

}  // namespace Tanks::Menu