#include "menu.h"
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

namespace Tanks::Menu {

std::string convertButtonTypeToString(const ButtonType type) {
    static std::unordered_map<ButtonType, std::string> dictionary{
        {ButtonType::NEW_GAME, "NEW GAME"},
        {ButtonType::EXIT, "EXIT"},
        {ButtonType::RESUME, "RESUME"},
        {ButtonType::SETTINGS, "SETTINGS"},
        {ButtonType::CREATE_MAP, "CREATE MAP"},
        {ButtonType::RATING, "RATING"},
        {ButtonType::PAUSE, "PAUSE"}};
    assert(dictionary.find(type) != dictionary.end());
    return dictionary[type];
}

Menu::Menu(const int menuWidth,
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
    auto title = std::make_unique<MenuInscription>(convertButtonTypeToString(buttonTypes[0]),
                                                 currentCoordinates);

    const size_t menuHeight = static_cast<size_t>(title->getSize().y) +
                              marginFromTitle + buttonsCount * buttonsHeight +
                              (buttonsCount - 1) * marginBetweenButtons;
    const size_t marginTop = (WINDOW_HEIGHT - menuHeight) / 2;
    currentCoordinates.y = static_cast<float>(marginTop);

    title->text.setPosition(
        {static_cast<float>(
             currentCoordinates.x +
                 static_cast<float>(menuWidth - title->getPosition().x) / 2),
         currentCoordinates.y});  // Centralize

    currentCoordinates.y += static_cast<float>(
        marginFromTitle + title->getPosition().y);

    items.emplace_back(std::move(title));
    for (int i = 0; i < buttonsCount; ++i) {
        auto content = std::make_unique<MenuInscription>(convertButtonTypeToString(buttonTypes[i]), currentCoordinates);
        items.emplace_back(std::make_unique<MenuButton>(std::move(content),
            currentCoordinates,
            sf::Vector2<float>(static_cast<float>(menuWidth),
                               static_cast<float>(buttonsHeight)),
            buttonsStandardColor, buttonsHoverColor, buttonTypes[i]));
        currentCoordinates.y +=
            static_cast<float>(items[i]->getSize().y) + marginBetweenButtons;
    }
}

void Menu::draw(sf::RenderWindow &window) const {
    for (const auto &item : items) {
        item->draw(window);
    }
}

void Menu::addMenuItem(std::unique_ptr<MenuItem> &&item) {
    items.emplace_back(std::move(item));
}

const std::vector<std::unique_ptr<MenuItem>> &Menu::getItems() const {
    return items;
}

MenuInscription::MenuInscription(const std::string &inscription, const sf::Vector2<float> &coordinates) {
    text.setString(inscription);
    text.setPosition(coordinates);
}

void MenuInscription::draw(sf::RenderWindow &window) const {
    window.draw(text);
}

sf::Vector2<int> MenuInscription::getSize() const {
    auto res = text.getGlobalBounds();
    return sf::Vector2<int>{static_cast<int>(res.width), static_cast<int>(res.height)};
}

sf::Vector2<int> MenuInscription::getPosition() const {
    return static_cast<sf::Vector2<int>>(text.getPosition());
}

void MenuInscription::setPosition(sf::Vector2<int> newPosition) {
    text.setPosition(static_cast<sf::Vector2<float>>(newPosition));
}

MenuPicture::MenuPicture(const std::string &path, const sf::Vector2<float> &coordinates) {
    image.loadFromFile(path);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setPosition(coordinates);
}

sf::Vector2<int> MenuPicture::getPosition() const {
    return static_cast<sf::Vector2<int>>(sprite.getPosition());
}

sf::Vector2<int> MenuPicture::getSize() const {
    return static_cast<sf::Vector2<int>>(image.getSize());
}

void MenuPicture::setPosition(sf::Vector2<int> newPosition) {
    sprite.setPosition(static_cast<sf::Vector2<float>>(newPosition));
}

void MenuPicture::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
}

MenuButton::MenuButton(std::unique_ptr<MenuItem> &&content_,
                       const sf::Vector2<float> &coordinates,
                       const sf::Vector2<float> &rectangleSize,
                       const sf::Color &rectangleStandardColor_,
                       const sf::Color &rectangleHoverColor_,
                       ButtonType type_)
    : content(std::move(content_)),
      rectangle(rectangleSize),
      rectangleStandardColor(rectangleStandardColor_),
      rectangleHoverColor(rectangleHoverColor_),
      type(type_) {
    rectangle.setFillColor(rectangleStandardColor);
    rectangle.setPosition(coordinates);
    content->setPosition({
            static_cast<int>(coordinates.x) +
            static_cast<int>(
                (rectangleSize.x -
                 static_cast<float>(content->getPosition().x)) /
                2),
            static_cast<int>(coordinates.y) +
            static_cast<int>(
                (rectangleSize.y -
                 static_cast<float>(content->getPosition().y)) /
                2)});  // Centralize
}

void MenuButton::draw(sf::RenderWindow &window) const {
    window.draw(rectangle);
    content->draw(window);
    rectangle.setFillColor(
        rectangleStandardColor);  // recover after possible hover;
}

ButtonType MenuButton::getType() const {
    return type;
}

void MenuButton::hover() {
    rectangle.setFillColor(rectangleHoverColor);
}

sf::Vector2<int> MenuButton::getPosition() const {
    return static_cast<sf::Vector2<int>>(rectangle.getPosition());
}

sf::Vector2<int> MenuButton::getSize() const {
    return static_cast<sf::Vector2<int>>(rectangle.getSize());
}

void MenuButton::setPosition(sf::Vector2<int> newPosition) {
    rectangle.setPosition(static_cast<sf::Vector2<float>>(newPosition));
}

}  // namespace Tanks::Menu