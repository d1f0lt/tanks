#include "menu.h"
#include "controller.h"
#include <cassert>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <thread>

namespace Tanks::Menu {

Menu::Menu(size_t menuWidth,
           const InscriptionInfo &titleInfo,
           const InscriptionInfo &inscriptionsInfo,
           const std::vector<ButtonWithType> &buttonsInfo) {
    const size_t buttonsCount = buttonsInfo.size();
    const static size_t marginBetweenButtons = 10;
    const static size_t marginFromTitle = marginBetweenButtons * 8;
    const size_t marginLeft = (WINDOW_WIDTH - menuWidth) / 2;

    sf::Vector2<float> currentCoordinates = {static_cast<float>(marginLeft), 0};

    // Title
    auto title =
        std::make_unique<MenuInscription>(titleInfo, currentCoordinates);

    const size_t menuHeight =
        static_cast<size_t>(title->getSize().y) + marginFromTitle +
        buttonsCount * static_cast<size_t>(buttonsInfo[0].getSize().y) +
        (buttonsCount - 1) * marginBetweenButtons;
    const size_t marginTop = (WINDOW_HEIGHT - menuHeight) / 2;

    title->setPosition(sf::Vector2<float>{0, static_cast<float>(marginTop)});
    title->centralizeByWidth({marginLeft, marginLeft + menuWidth});
    title->setStandardPosition(title->getPosition());

    currentCoordinates.y = static_cast<float>(marginTop) +
                           static_cast<float>(marginFromTitle) +
                           title->getSize().y;

    items.emplace_back(std::move(title));

    // buttons
    for (int i = 0; i < buttonsCount; ++i) {
        auto btnInfo = buttonsInfo[i];
        InscriptionInfo inscriptionInfo{
            convertButtonTypeToString(btnInfo.getType()),
            inscriptionsInfo.characterSize, inscriptionsInfo.textColor};
        auto content = std::make_unique<MenuInscription>(inscriptionInfo,
                                                         currentCoordinates);
        items.emplace_back(std::make_unique<MenuButton>(
            std::move(content), currentCoordinates, btnInfo));
        currentCoordinates.y += static_cast<float>(items[i + 1]->getSize().y) +
                                marginBetweenButtons;
    }
}

Menu::Menu(size_t menuWidth,
           const InscriptionInfo &titleInfo,
           const std::vector<InscriptionInfo> &inscriptions,
           const std::string &path,
           size_t quantityPerLine,
           Button btnInfo) {
    sf::Vector2<float> sizeOfOne;
    if (btnInfo.getSize() == sf::Vector2<float>{0, 0}) {
        sf::Image image;
        image.loadFromFile(path + "item1.png");
        sizeOfOne = sf::Vector2<float>{
            static_cast<float>(image.getSize().x),
            static_cast<float>(image.getSize().y + 2 * TEXT_MARGIN) +
                MenuInscription{inscriptions[0], {0, 0}}.getSize().y};
        btnInfo.setSize(sizeOfOne);
    } else {
        sizeOfOne = btnInfo.getSize();
    }

    const size_t itemsCount = inscriptions.size();
    const size_t marginLeft = (WINDOW_WIDTH - menuWidth) / 2;
    const size_t marginBetweenButtons =
        menuWidth - static_cast<int>(sizeOfOne.x) * quantityPerLine;
    const static size_t marginFromTitle = 80;

    sf::Vector2<float> currentCoordinates{static_cast<float>(marginLeft), 0};

    auto title =
        std::make_unique<MenuInscription>(titleInfo, currentCoordinates);

    const size_t linesQuantity = ceil(static_cast<double>(itemsCount) /
                                      static_cast<double>(quantityPerLine));
    const size_t itemsQuantityInLastLine =
        itemsCount - ((linesQuantity - 1) * quantityPerLine);
    const size_t menuHeight = static_cast<int>(title->getSize().y) +
                              marginFromTitle +
                              linesQuantity * static_cast<int>(sizeOfOne.y);
    const size_t marginTop = (WINDOW_HEIGHT - menuHeight) / 2;

    title->setPosition(sf::Vector2<float>(0, static_cast<float>(marginTop)));
    title->centralizeByWidth({marginLeft, marginLeft + menuWidth});
    title->setStandardPosition(title->getPosition());

    currentCoordinates.y = static_cast<float>(marginTop) + title->getSize().y +
                           static_cast<float>(marginFromTitle);

    items.emplace_back(std::move(title));

    const std::string filename = path + "item";
    const static sf::Color textColor(0, 0, 0);
    for (int i = 0; i < linesQuantity; ++i) {
        currentCoordinates.x = static_cast<float>(marginLeft);
        for (int j = 1; j <= (i + 1 != linesQuantity ? quantityPerLine
                                                     : itemsQuantityInLastLine);
             ++j) {
            const std::string currentItem =
                std::to_string(i * quantityPerLine + j);
            auto picture = std::make_unique<MenuPicture>(
                filename + currentItem + ".png", currentCoordinates);
            auto description = std::make_unique<MenuInscription>(
                inscriptions[i * quantityPerLine + j - 1], currentCoordinates);
            auto item = std::make_unique<MenuPictureWithDescription>(
                std::move(picture), std::move(description), currentCoordinates,
                btnInfo);
            items.emplace_back(std::move(item));
            currentCoordinates.x +=
                sizeOfOne.x + static_cast<float>(marginBetweenButtons);
            assert(items[i * quantityPerLine + j]->getSize() == sizeOfOne);
        }
        currentCoordinates.y +=
            sizeOfOne.y + static_cast<float>(marginBetweenButtons);
    }
}

void Menu::draw(sf::RenderWindow &window) const {
    for (const auto &item : items) {
        item->draw(window);
    }
}

namespace {

std::unique_ptr<MenuButton> initIcon(const sf::Image &image,
                                     ButtonType type,
                                     const sf::Vector2<float> &coordinates,
                                     int padding) {
    const sf::Vector2<float> rectangleSize{
        static_cast<float>(image.getSize().x + padding),
        static_cast<float>(image.getSize().y + padding)};
    auto icon = std::make_unique<MenuPicture>(image, coordinates);
    ButtonWithType btnInfo(
        type, sf::Vector2<float>(rectangleSize.x, rectangleSize.y),
        sf::Color(0, 0, 0, 0), sf::Color(128, 128, 128, 128));
    auto item =
        std::make_unique<MenuButton>(std::move(icon), coordinates, btnInfo);
    return item;
}

}  // namespace

void Menu::addIconToLeftUpCorner(const std::string &filename, ButtonType type) {
    sf::Image image;
    image.loadFromFile(filename);
    const static int margin = 5;
    const static sf::Vector2<float> coordinates{2 * margin, 2 * margin};
    addMenuItem(std::move(initIcon(image, type, coordinates, margin)));
}

void Menu::addIconToLeftLowerCorner(const std::string &filename,
                                    ButtonType type) {
    sf::Image image;
    image.loadFromFile(filename);
    const int margin = 5;
    sf::Vector2<float> coordinates(
        margin,
        static_cast<float>(WINDOW_HEIGHT - image.getSize().y - 3 * margin));
    addMenuItem(std::move(initIcon(image, type, coordinates, 2 * margin)));
}

void Menu::addMenuItem(std::unique_ptr<MenuItem> &&item) {
    items.emplace_back(std::move(item));
}

const std::vector<std::unique_ptr<MenuItem>> &Menu::getItems() const {
    return items;
}

void Menu::moveItems(float distance) {
    for (auto &item : items) {
        item->move(sf::Vector2<float>(distance, 0));
    }
}

void Menu::animation(sf::RenderWindow &window,
                     const sf::Sprite &backgroundSprite,
                     int stepsAmount,
                     float speed_) {
    for (int i = 0; i < stepsAmount; ++i) {
        moveItems(speed_);
        window.clear();
        window.draw(backgroundSprite);
        this->draw(window);
        window.display();
    }
}

static constexpr int speed = 3;

void Menu::flyOutFromLeft(sf::RenderWindow &window,
                          const sf::Sprite &backgroundSprite) {
    assert(!items.empty());
    const auto stepsAmount =
        (items[0]->getStandardPosition().x - items[0]->getPosition().x) / speed;
    assert(stepsAmount == static_cast<int>(stepsAmount));
    assert(stepsAmount > 0);
    animation(window, backgroundSprite, static_cast<int>(stepsAmount), speed);
}

void Menu::flyAwayToLeft(sf::RenderWindow &window,
                         const sf::Sprite &backgroundSprite) {
    assert(!items.empty());
    float maxPositionX = 0;
    for (const auto &item : items) {
        auto pos = item->getPosition().x + item->getSize().x;
        maxPositionX = std::max(pos, maxPositionX);
    }
    assert(maxPositionX > 0 && maxPositionX < WINDOW_WIDTH);
    const int stepsAmount = static_cast<int>(std::ceil(maxPositionX / speed));
    animation(window, backgroundSprite, stepsAmount, -speed);
}

void Menu::flyOutFromRight(sf::RenderWindow &window,
                           const sf::Sprite &backgroundSprite) {
    assert(!items.empty());
    const auto stepsAmount =
        (items[0]->getPosition().x - items[0]->getStandardPosition().x) / speed;
    assert(stepsAmount == static_cast<int>(stepsAmount));
    assert(stepsAmount > 0);
    animation(window, backgroundSprite, static_cast<int>(stepsAmount), -speed);
}

void Menu::flyAwayToRight(sf::RenderWindow &window,
                          const sf::Sprite &backgroundSprite) {
    assert(!items.empty());
    float minPositionX = WINDOW_WIDTH;
    for (const auto &item : items) {
        auto pos = item->getPosition().x;
        minPositionX = std::min(pos, minPositionX);
    }
    assert(minPositionX < WINDOW_WIDTH && minPositionX >= 0);
    const int stepsAmount =
        static_cast<int>(std::ceil((WINDOW_WIDTH - minPositionX) / speed));
    animation(window, backgroundSprite, stepsAmount, speed);
}

void Menu::flyAwayToRight() {
    assert(!items.empty());
    float minPositionX = WINDOW_WIDTH;
    for (const auto &item : items) {
        auto pos = item->getPosition().x;
        minPositionX = std::min(pos, minPositionX);
    }
    assert(minPositionX < WINDOW_WIDTH && minPositionX >= 0);
    const int stepsAmount =
        static_cast<int>(std::ceil((WINDOW_WIDTH - minPositionX) / speed));
    moveItems(static_cast<float>(stepsAmount * speed));
}

const MenuButton *Menu::showMenu(sf::RenderWindow &window, const sf::Sprite &backgroundSprite) {
    while (window.isOpen()) {
        // catch event
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (const auto res =
                Tanks::MenuController::control(getItems(), window, event);
            res != std::nullopt) {
            return res.value();
        }

        // redraw
        window.clear();
        window.draw(backgroundSprite);
        this->draw(window);
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    return nullptr;
}


}  // namespace Tanks::Menu