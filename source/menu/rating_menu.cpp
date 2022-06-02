#include "menu/rating_menu.h"
#include <cassert>
#include <memory>

namespace Tanks::Menu {

namespace {

void addItemToVector(std::vector<std::unique_ptr<MenuInscription>> &vec,
                     const InscriptionInfo &info) {
    vec.emplace_back(
        std::make_unique<MenuInscription>(info, sf::Vector2<float>{0, 0}));
}

std::unique_ptr<OwningRectangle> initRectangle(const std::string &titleText,
                                               const sf::Color &textColor,
                                               int killsCount,
                                               int deathCount) {
    const static size_t titleCharacterSize = 60;
    InscriptionInfo titleInfo{titleText, titleCharacterSize, textColor};
    auto title =
        std::make_unique<MenuInscription>(titleInfo, sf::Vector2<float>{0, 0});

    const static size_t textCharacterSize = 40;
    std::vector<std::unique_ptr<MenuInscription>> leftItems;
    std::vector<std::unique_ptr<MenuInscription>> rightItems;

    InscriptionInfo info{"Number of kills:", textCharacterSize, textColor};
    addItemToVector(leftItems, info);
    info.inscription = std::to_string(killsCount);
    addItemToVector(rightItems, info);

    info.inscription = "Number of deaths:";
    addItemToVector(leftItems, info);
    info.inscription = std::to_string(deathCount);
    addItemToVector(rightItems, info);

    info.inscription = "Rating:";
    addItemToVector(leftItems, info);
    info.inscription = std::to_string((deathCount == 0 ? killsCount : killsCount / deathCount));
    addItemToVector(rightItems, info);

    static sf::Color backgroundColor{0, 0, 0, 192};

    return std::make_unique<OwningRectangle>(
        std::move(title), std::move(leftItems), std::move(rightItems),
        backgroundColor, sf::Vector2<float>{0, 0});
}

Menu initMenu(const std::string &imagesPath, PlayerInfo &playerInfo) {
    auto &rating = playerInfo.rating;
    const static sf::Color textColor{63, 87, 210};

    const static size_t titleCharacterSize = 80;
    const static size_t marginFromTitle = 80;
    InscriptionInfo titleInfo{playerInfo.general.name, titleCharacterSize,
                              textColor};
    auto title = std::make_unique<MenuInscription>(
        titleInfo, sf::Vector2<float>{100, 100});

    auto singlePlayer = initRectangle("Single player", textColor,
                                      rating.singlePlayerKills,
                                      rating.singlePlayerDeath);
    auto multiplayer = initRectangle("Multiplayer", textColor,
                                     rating.multiPlayerKills,
                                     rating.multiPlayerDeath);

    const static size_t marginBetweenStats = 200;
    sf::Vector2<float> curCoordinates{
        (WINDOW_WIDTH - singlePlayer->getSize().x - multiplayer->getSize().x -
         marginBetweenStats)/2, (WINDOW_HEIGHT - title->getSize().y - marginFromTitle - singlePlayer->getSize().y) / 2};
    title->setPosition(curCoordinates);
    curCoordinates.y += title->getSize().y + marginFromTitle;
    singlePlayer->setPosition(curCoordinates);
    singlePlayer->setStandardPosition(curCoordinates);
    curCoordinates.x += singlePlayer->getSize().x + marginBetweenStats;
    multiplayer->setPosition(curCoordinates);
    multiplayer->setStandardPosition(curCoordinates);
    title->centralizeByWidth({singlePlayer->getPosition().x, multiplayer->getPosition().x + multiplayer->getSize().x});
    title->setStandardPosition(title->getPosition());

    Menu menu;

    menu.addMenuItem(std::move(title));
    menu.addMenuItem(std::move(singlePlayer));
    menu.addMenuItem(std::move(multiplayer));

    menu.addIconToLeftUpCorner(imagesPath + "return.png", ButtonType::RETURN);

    return menu;
}

}  // namespace

void showRatingMenu(sf::RenderWindow &window,
                    const sf::Sprite &backgroundSprite,
                    PlayerInfo &info) {
    const static std::string imagesPath = "../images/menu/";

    Menu menu(initMenu(imagesPath, info));

    menu.flyAwayToRight();
    menu.flyOutFromRight(window, backgroundSprite);

    while (window.isOpen()) {
        const auto *res = menu.showMenu(window, backgroundSprite);
        switch (res->getType()) {
            case ButtonType::RETURN:
                menu.flyAwayToRight(window, backgroundSprite);
                return;
            default:
                assert(false);
        }
    }
}

}  // namespace Tanks::Menu