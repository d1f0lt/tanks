#include "menu/settings_menu.h"
#include <cassert>
#include "menu/slider_bar.h"
#include <cmath>

namespace Tanks::Menu {

namespace {

Menu initMenu(PlayerInfo &info, const std::string &imagesPath) {
    const static size_t amount = 5;
    const static sf::Vector2<float> sizeOfOne{96, 96};
    const static size_t marginFromSliderBar = 30;
    auto &settings = info.settings;
    const static sf::Color textColor{63, 87, 210};

    // title
    const static std::string titleText = "SETTINGS";
    const static size_t titleCharacterSize = 80;
    InscriptionInfo titleInfo{titleText, titleCharacterSize, textColor};
    const static size_t marginFromTitle = 80;

    auto title =
        std::make_unique<MenuInscription>(titleInfo, sf::Vector2<float>{0, 0});

    titleInfo.inscription = "Music";
    titleInfo.characterSize = 40;
    titleInfo.textColor = sf::Color{255, 255, 255};
    auto musicVolume = std::make_unique<SliderBar>(
        titleInfo, sf::Vector2<float>{0, 0}, MAX_VOLUME, settings.musicVolume, SliderBarType::MUSIC);

    titleInfo.inscription = "Volume";
    auto soundVolume = std::make_unique<SliderBar>(
        titleInfo, sf::Vector2<float>{0, 0}, MAX_VOLUME, settings.soundsVolume, SliderBarType::SOUND);



    sf::Vector2<float> curCoordinates{
        (WINDOW_WIDTH) / 2,
        (WINDOW_HEIGHT) / 2};

    title->setPosition(curCoordinates);
    title->centralizeByWidth(
        {curCoordinates.x, curCoordinates.x + tankSpeed->getSize().x});
    title->setStandardPosition(title->getPosition());
    curCoordinates.y += title->getSize().y + marginFromTitle;
    musicVolume->axis.setPosition(curCoordinates);
    musicVolume->coordinates = curCoordinates;
    musicVolume->setSliderVolume(settings.musicVolume);

    curCoordinates.y += musicVolume->sliderHeight() + marginFromProgressBar;
    soundVolume->axis.setPosition(curCoordinates);
    soundVolume->coordinates = curCoordinates;
    soundVolume->setSliderVolume(settings.soundVolume);

    Menu menu;

    menu.addMenuItem(std::move(title));
    menu.addMenuItem(std::move(musicVolume));
    menu.addMenuItem(std::move(soundVolume));

    menu.addIconToLeftUpCorner(imagesPath + "return.png", ButtonType::RETURN);

    return menu;
}

void updateVolume(PlayerSettings &settings, const SliderBar *sliderBar) {
    switch (sliderBar->getType()) {
        case SliderBarType::MUSIC:
            settings.musicVolume = sliderBar->getCurrentVolume;
            break;
        case SliderBarType::SOUND:
            settings.soundsVolume = sliderBar->getCurrentVolume;
            break;
        default:
            assert(false);
    }
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
        const auto *const res = menu.showMenu(window, backgroundSprite);
        switch (res->getType()) {
            case ButtonType::RETURN:
                return;
            break;
            default:
                assert(false);
        }
    }
}

}  // namespace Tanks::Menu 