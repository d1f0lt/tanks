#include "menu.h"
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

namespace Tanks::Menu {

std::string convertButtonTypeToString(const ButtonType type) {
    static std::unordered_map<ButtonType, std::string> dictionary{
        {ButtonType::NEW_GAME, "new_game"},
        {ButtonType::EXIT, "exit"},
        {ButtonType::RESUME, "resume"},
        {ButtonType::SETTINGS, "settings"},
        {ButtonType::CREATE_MAP, "create_map"},
        {ButtonType::RATING, "rating"},
        {ButtonType::PAUSE, "pause"}};
    assert(dictionary.find(type) != dictionary.end());
    return dictionary[type];
}

Menu::Menu(const std::string &imagesPath,
           const int menuWidth,
           const std::vector<ButtonType> &buttonTypes,
           const int buttonsHeight,
           const sf::Color &buttonsStandardColor,
           const sf::Color &buttonsHoverColor) {
    const size_t buttonsCount = buttonTypes.size();
    const int marginBetweenButtons = 10;
    const int marginFromTitle = marginBetweenButtons * 8;
    const int marginLeft = (WINDOW_WIDTH - menuWidth) / 2;

    sf::Vector2<float> currentCoordinates = {static_cast<float>(marginLeft), 0};

    // Title
    items.emplace_back(std::make_unique<MenuItem>(imagesPath + "title.png",
                                                  currentCoordinates));

    const size_t menuHeight = items[0]->inscriptionImage.getSize().y +
                              marginFromTitle + buttonsCount * buttonsHeight +
                              (buttonsCount - 1) * marginBetweenButtons;
    const size_t marginTop = (WINDOW_HEIGHT - menuHeight) / 2;
    currentCoordinates.y = static_cast<float>(marginTop);

    items[0]->inscriptionSprite.setPosition(
        {static_cast<float>(
             static_cast<int>(currentCoordinates.x) +
             static_cast<int>(
                 (menuWidth - items[0]->inscriptionImage.getSize().x) / 2)),
         currentCoordinates.y});  // Centralize

    // Buttons
    currentCoordinates.y += static_cast<float>(
        marginFromTitle + items[0]->inscriptionImage.getSize().y);
    for (size_t i = 0; i < buttonsCount; ++i) {
        items.emplace_back(std::make_unique<MenuButton>(
            imagesPath + convertButtonTypeToString(buttonTypes[i]) + ".png",
            currentCoordinates,
            sf::Vector2<float>(static_cast<float>(menuWidth),
                               static_cast<float>(buttonsHeight)),
            buttonsStandardColor, buttonsHoverColor, buttonTypes[i]));
        currentCoordinates.y +=
            static_cast<float>(buttonsHeight) + marginBetweenButtons;
    }
}

void Menu::drawMenu(sf::RenderWindow &window) const {
    for (const auto &item : items) {
        item->drawSprite(window);
    }
}

void Menu::addMenuItem(std::unique_ptr<MenuItem> &&item) {
    items.emplace_back(std::move(item));
}

const std::vector<std::unique_ptr<MenuItem>> &Menu::getItems() const {
    return items;
}

MenuItem::MenuItem(const std::string &path,
                   const sf::Vector2<float> &coordinates) {
    inscriptionImage.loadFromFile(path);
    inscriptionTexture.loadFromImage(inscriptionImage);
    inscriptionSprite.setTexture(inscriptionTexture);
    inscriptionSprite.setPosition(coordinates);
}

void MenuItem::drawSprite(sf::RenderWindow &window) const {
    window.draw(inscriptionSprite);
}

MenuButton::MenuButton(const std::string &path,
                       const sf::Vector2<float> &coordinates,
                       const sf::Vector2<float> &rectangleSize,
                       const sf::Color &rectangleStandardColor_,
                       const sf::Color &rectangleHoverColor_,
                       ButtonType type_)
    : MenuItem(path, coordinates),
      rectangle(rectangleSize),
      rectangleStandardColor(rectangleStandardColor_),
      rectangleHoverColor(rectangleHoverColor_),
      type(type_) {
    rectangle.setFillColor(rectangleStandardColor);
    rectangle.setPosition(coordinates);
    inscriptionSprite.setPosition(
        static_cast<float>(
            static_cast<int>(coordinates.x) +
            static_cast<int>(
                (rectangleSize.x -
                 static_cast<float>(inscriptionImage.getSize().x)) /
                2)),
        static_cast<float>(
            static_cast<int>(coordinates.y) +
            static_cast<int>(
                (rectangleSize.y -
                 static_cast<float>(inscriptionImage.getSize().y)) /
                2)));  // Centralize
}

void MenuButton::drawSprite(sf::RenderWindow &window) const {
    window.draw(rectangle);
    window.draw(inscriptionSprite);
    rectangle.setFillColor(
        rectangleStandardColor);  // recover after possible hover;
}

ButtonType MenuButton::getType() const {
    return type;
}

void MenuButton::hover() {
    rectangle.setFillColor(rectangleHoverColor);
}

}  // namespace Tanks::Menu