#include "menu/settings_menu.h"
#include "menu/menu_controller.h"
#include <cassert>
#include "menu/slider_bar.h"
#include <cmath>
#include <iostream>

namespace Tanks::Menu {

namespace {

Menu initMenu(PlayerInfo &info, const std::string &imagesPath) {
    const static size_t marginFromSliderBar = 60;
    auto &settings = info.settings;
    const static sf::Color textColor{63, 87, 210};

    // title
    const static std::string titleText = "SETTINGS";
    const static size_t titleCharacterSize = 80;
    InscriptionInfo titleInfo{titleText, titleCharacterSize, textColor};
    const static size_t marginFromTitle = 140;

    auto title =
        std::make_unique<MenuInscription>(titleInfo, sf::Vector2<float>{0, 0});

    titleInfo.inscription = "Music volume:";
    titleInfo.characterSize = 40;
    titleInfo.textColor = {63, 87, 210};
    auto musicVolume = std::make_unique<SliderBar>(
        titleInfo, sf::Vector2<float>{0, 0}, MAX_VOLUME, settings.musicVolume, SliderBarType::MUSIC);

    titleInfo.inscription = "Sounds volume:";
    auto soundVolume = std::make_unique<SliderBar>(
        titleInfo, sf::Vector2<float>{0, 0}, MAX_VOLUME, settings.soundsVolume, SliderBarType::SOUND);



    sf::Vector2<float> curCoordinates{
        (WINDOW_WIDTH) / 2.8,
        (WINDOW_HEIGHT) / 4};

    title->setPosition(curCoordinates);
    title->centralizeByWidth(
        {curCoordinates.x, curCoordinates.x + 600});
    title->setStandardPosition(title->getPosition());
    curCoordinates.y += title->getSize().y + marginFromTitle;
    musicVolume->setPosition(curCoordinates);
    // std::cout << musicVolume->getPosition().x << " " << musicVolume->getPosition().y << '\n';

    curCoordinates.y += musicVolume->getSize().y + marginFromSliderBar;
    soundVolume->setPosition(curCoordinates);
    Menu menu;

    menu.addMenuItem(std::move(title));
    menu.addMenuItem(std::move(musicVolume));
    menu.addMenuItem(std::move(soundVolume));

    menu.addIconToLeftUpCorner(imagesPath + "return.png", ButtonType::RETURN);

    return menu;
}

}

void showSettingsMenu(sf::RenderWindow &window,
                     const sf::Sprite &backgroundSprite,
                     PlayerInfo &info) {
    const static std::string imagesPath = "../images/menu/";

    Menu menu(initMenu(info, imagesPath));

    menu.flyAwayToRight();
    menu.flyOutFromRight(window, backgroundSprite);


    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        Tanks::MenuController::sliderMove(menu, window, info.settings);

        if (const auto res =
                Tanks::MenuController::control(menu, window, event);
            res != std::nullopt) {
            assert(res.value()->getType() == ButtonType::RETURN);
            menu.flyAwayToRight(window, backgroundSprite);
            return;
        }

        window.clear();
        window.draw(backgroundSprite);
        menu.draw(window);
        window.display();
    }
}

}  // namespace Tanks::Menu 