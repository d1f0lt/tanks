#include "menu/upgrade_menu.h"
#include <cassert>
#include "menu/progress_bar.h"
#include <cmath>

namespace Tanks::Menu {

namespace {

int getPrice(size_t degree) {
    return static_cast<int>(std::pow(2, degree) * 1e2);
}

void addPrice(std::unique_ptr<ProgressBar> &progressBar,
              const std::string &imagesPath) {
    progressBar->getCurrentProgress();
    if (progressBar->getMaxProgress() - progressBar->getCurrentProgress() > 0) {
        auto coin = std::make_unique<MenuPicture>(imagesPath + "coin.png", 32,
                                                  1, sf::Vector2<float>{0, 0});
        InscriptionInfo priceInfo{
            std::to_string(getPrice(progressBar->getCurrentProgress())), 40,
            sf::Color{255, 255, 255}};
        auto price = std::make_unique<MenuInscription>(
            priceInfo, sf::Vector2<float>{0, 0});
        std::vector<std::unique_ptr<MenuItem>> res;
        res.emplace_back(std::move(coin));
        res.emplace_back(std::move(price));
        progressBar->addInfoToRightUpCorner(std::move(res));
    }
}

Menu initMenu(PlayerInfo &info, const std::string &imagesPath) {
    const static size_t amount = 5;
    const static sf::Vector2<float> sizeOfOne{96, 96};
    const static size_t marginFromProgressBar = 20;
    auto &skills = info.skills;
    const static sf::Color textColor{63, 87, 210};

    // title
    const static std::string titleText = "SKILLS";
    const static size_t titleCharacterSize = 80;
    InscriptionInfo titleInfo{titleText, titleCharacterSize, textColor};
    const static size_t marginFromTitle = 80;

    auto title =
        std::make_unique<MenuInscription>(titleInfo, sf::Vector2<float>{0, 0});

    titleInfo.inscription = "Tank speed";
    titleInfo.characterSize = 40;
    titleInfo.textColor = sf::Color{255, 255, 255};
    auto tankSpeed = std::make_unique<ProgressBar>(
        titleInfo, amount, skills.tankSpeed - DEFAULT_TANK_SPEED, sizeOfOne,
        sf::Vector2<float>{0, 0}, ProgressBarType::TANK_SPEED);
    addPrice(tankSpeed, imagesPath);

    titleInfo.inscription = "Bullet speed";
    auto bulletSpeed = std::make_unique<ProgressBar>(
        titleInfo, amount, skills.bulletSpeed - DEFAULT_BULLET_SPEED, sizeOfOne,
        sf::Vector2<float>{0, 0}, ProgressBarType::BULLET_SPEED);
    addPrice(bulletSpeed, imagesPath);

    titleInfo.inscription = "Reload speed";
    auto reloadTicks = std::make_unique<ProgressBar>(
        titleInfo, amount,
        static_cast<size_t>((DEFAULT_RELOAD_TICKS - skills.reloadTicks) / 10),
        sizeOfOne, sf::Vector2<float>{0, 0}, ProgressBarType::RELOAD_TICKS);
    addPrice(reloadTicks, imagesPath);

    assert(title->getSize().x <= tankSpeed->getSize().x);
    assert(tankSpeed->getSize() == bulletSpeed->getSize());
    assert(tankSpeed->getSize() == reloadTicks->getSize());

    sf::Vector2<float> curCoordinates{
        (WINDOW_WIDTH - tankSpeed->getSize().x) / 2,
        (WINDOW_HEIGHT - title->getSize().y - marginFromTitle -
         tankSpeed->getSize().y * 3 - 2 * marginFromProgressBar) /
            2};

    title->setPosition(curCoordinates);
    title->centralizeByWidth(
        {curCoordinates.x, curCoordinates.x + tankSpeed->getSize().x});
    title->setStandardPosition(title->getPosition());
    curCoordinates.y += title->getSize().y + marginFromTitle;
    tankSpeed->setPosition(curCoordinates);
    tankSpeed->setStandardPosition(curCoordinates);
    curCoordinates.y += tankSpeed->getSize().y + marginFromProgressBar;
    bulletSpeed->setPosition(curCoordinates);
    bulletSpeed->setStandardPosition(curCoordinates);
    curCoordinates.y += bulletSpeed->getSize().y + marginFromProgressBar;
    reloadTicks->setPosition(curCoordinates);
    reloadTicks->setStandardPosition(curCoordinates);

    std::vector<std::unique_ptr<MenuItem>> addingButtons;
    for (size_t i = 0; i < 3; ++i) {
        auto item = std::make_unique<MenuPicture>(imagesPath + "plus.png",
                                                  sf::Vector2<float>{0, 0});
        addingButtons.emplace_back(std::move(item));
    }
    const static sf::Color addingButtonsStandardColor{0, 0, 0, 0};
    const static sf::Color addingButtonsHoverColor{128, 128, 128, 128};

    ButtonWithType btnInfo{ButtonType::IMPROVE,
                           sf::Vector2<float>{tankSpeed->getSizeOfOne().x,
                                              tankSpeed->getSizeOfOne().y},
                           addingButtonsStandardColor, addingButtonsHoverColor};

    Menu menu;

    menu.addMenuItem(std::move(title));
    menu.addMenuItem(std::move(tankSpeed));
    menu.addMenuItem(std::move(bulletSpeed));
    menu.addMenuItem(std::move(reloadTicks));
    menu.addAddingButtons(1, std::move(addingButtons), btnInfo);

    menu.addPlayerInfo(info.general);
    menu.addIconToLeftUpCorner(imagesPath + "return.png", ButtonType::RETURN);

    return menu;
}

void updateSkills(PlayerSkills &skills, const ProgressBar *progressBar) {
    switch (progressBar->getType()) {
        case ProgressBarType::TANK_SPEED:
            skills.tankSpeed++;
            break;
        case ProgressBarType::BULLET_SPEED:
            skills.bulletSpeed++;
            break;
        case ProgressBarType::RELOAD_TICKS:
            skills.reloadTicks -= 10;
            break;
        default:
            assert(false);
    }
}

}

void showUpgradeMenu(sf::RenderWindow &window,
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
            case ButtonType::IMPROVE: {
                const auto *btn =
                    dynamic_cast<const MenuAdditionalButton *>(res);
                assert(btn != nullptr);
                const auto *progressBar = dynamic_cast<const ProgressBar*>(btn->getMainButton());
                assert(progressBar != nullptr);
                if (progressBar->getCurrentProgress() == progressBar->getMaxProgress()) {
                    continue;
                }
                int price = getPrice(progressBar->getCurrentProgress());
                if (price <= info.general.money) {
                    info.general.money -= price;
                    updateSkills(info.skills, progressBar);
                    menu = initMenu(info, imagesPath);
                }
            } break;
            default:
                assert(false);
        }
    }
}

}  // namespace Tanks::Menu