#include "menu/rating_menu.h"
#include <cassert>
#include <cmath>
#include <memory>

namespace Tanks::Menu {

namespace {

void addItemToVector(std::vector<std::unique_ptr<MenuInscription>> &vec,
                     const InscriptionInfo &info) {
    vec.emplace_back(
        std::make_unique<MenuInscription>(info, sf::Vector2<float>{0, 0}));
}

std::string toString(double num) {
    num = round(num * 1e2) / 1e2;
    std::string res = std::to_string(num);
    while (res.back() == '0') {
        res.pop_back();
    }
    return res;
}

template <typename T>
void initInscription(
    const std::string &newInscription,
    InscriptionInfo &info,
    T value,
    std::vector<std::unique_ptr<MenuInscription>> &leftItems,
    std::vector<std::unique_ptr<MenuInscription>> &rightItems) {
    info.inscription = newInscription;
    addItemToVector(leftItems, info);
    if constexpr (!(std::is_same<T, std::string>::value)) {
        info.inscription = std::to_string(value);
    } else {
        info.inscription = value;
    }
    addItemToVector(rightItems, info);
}

std::unique_ptr<OwningRectangle> initRectangle(const std::string &titleText,
                                               const sf::Color &textColor,
                                               PlayerRating &rating) {
    const static size_t titleCharacterSize = 60;
    InscriptionInfo titleInfo{titleText, titleCharacterSize, textColor};
    auto title =
        std::make_unique<MenuInscription>(titleInfo, sf::Vector2<float>{0, 0});

    const static size_t textCharacterSize = 40;
    std::vector<std::unique_ptr<MenuInscription>> leftItems;
    std::vector<std::unique_ptr<MenuInscription>> rightItems;

    InscriptionInfo info{"", textCharacterSize, textColor};
    initInscription("Kills:", info, rating.kills, leftItems, rightItems);

    initInscription("Deaths:", info, rating.deaths, leftItems, rightItems);

    std::string value =
        (rating.deaths == 0
             ? std::to_string(rating.kills)
             : toString(static_cast<double>(rating.kills) / rating.deaths));
    initInscription("Rating:", info, value, leftItems, rightItems);

    initInscription("Wins:", info, rating.wins, leftItems, rightItems);

    initInscription("Defeats:", info, rating.defeats, leftItems, rightItems);

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

    auto singlePlayer =
        initRectangle("Single player", textColor, rating.singlePlayer);
    auto multiplayer =
        initRectangle("Multiplayer", textColor, rating.multiplayer);

    const static size_t marginBetweenStats = 200;
    sf::Vector2<float> curCoordinates{
        (WINDOW_WIDTH - singlePlayer->getSize().x - multiplayer->getSize().x -
         marginBetweenStats) /
            2,
        (WINDOW_HEIGHT - title->getSize().y - marginFromTitle -
         singlePlayer->getSize().y) /
            2};
    title->setPosition(curCoordinates);
    curCoordinates.y += title->getSize().y + marginFromTitle;
    singlePlayer->setPosition(curCoordinates);
    singlePlayer->setStandardPosition(curCoordinates);
    curCoordinates.x += singlePlayer->getSize().x + marginBetweenStats;
    multiplayer->setPosition(curCoordinates);
    multiplayer->setStandardPosition(curCoordinates);
    title->centralizeByWidth(
        {singlePlayer->getPosition().x,
         multiplayer->getPosition().x + multiplayer->getSize().x});
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
