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
        {ButtonType::PAUSE, "PAUSE"},
        {ButtonType::UPGRADE, "UPGRADE"},
        {ButtonType::SINGLE_PLAYER, "SINGLE PLAYER"},
        {ButtonType::MULTIPLAYER, "MULTIPLAYER"}};
    assert(dictionary.find(type) != dictionary.end());
    return dictionary[type];
}

Menu::Menu(int menuWidth,
           const InscriptionInfo &titleInfo,
           const InscriptionInfo &inscriptionsInfo,
           const std::vector<ButtonInfo> &buttonsInfo) {
    const size_t buttonsCount = buttonsInfo.size();
    const int marginBetweenButtons = 10;
    const int marginFromTitle = marginBetweenButtons * 8;
    const int marginLeft = (WINDOW_WIDTH - menuWidth) / 2;

    sf::Vector2<float> currentCoordinates = {static_cast<float>(marginLeft), 0};

    // Title
    auto title =
        std::make_unique<MenuInscription>(titleInfo, currentCoordinates);

    const size_t menuHeight = static_cast<size_t>(title->getSize().y) +
                              marginFromTitle +
                              buttonsCount * buttonsInfo[0].height +
                              (buttonsCount - 1) * marginBetweenButtons;
    const size_t marginTop = (WINDOW_HEIGHT - menuHeight) / 2;
    currentCoordinates.y = static_cast<float>(marginTop);
    title->text.setPosition(
        {currentCoordinates.x +
             (static_cast<float>(menuWidth) - title->getSize().x) / 2,
         currentCoordinates.y});  // Centralize

    currentCoordinates.y +=
        static_cast<float>(marginFromTitle + title->getSize().y);

    items.emplace_back(std::move(title));

    // buttons
    for (int i = 0; i < buttonsCount; ++i) {
        auto btnInfo = buttonsInfo[i];
        InscriptionInfo info{convertButtonTypeToString(btnInfo.type),
                             inscriptionsInfo.characterSize,
                             inscriptionsInfo.textColor};
        auto content =
            std::make_unique<MenuInscription>(info, currentCoordinates);
        items.emplace_back(std::make_unique<MenuButton>(
            std::move(content), currentCoordinates,
            sf::Vector2<float>(static_cast<float>(menuWidth),
                               static_cast<float>(btnInfo.height)),
            btnInfo.standardColor, btnInfo.hoverColor, btnInfo.type));
        currentCoordinates.y += static_cast<float>(items[i + 1]->getSize().y) +
                                marginBetweenButtons;
    }
}

void Menu::draw(sf::RenderWindow &window) const {
    for (const auto &item : items) {
        item->draw(window);
    }
}

void Menu::addIconToLeftUpCorner(const std::string &filename, ButtonType type) {
    sf::Image image;
    image.loadFromFile(filename);
    const int margin = 5;
    const sf::Vector2<float> coordinates{2 * margin, 2 * margin};
    const sf::Vector2<float> rectangleSize{
        static_cast<float>(image.getSize().x) + margin,
        static_cast<float>(image.getSize().y) + margin};
    auto pauseSprite =
        std::make_unique<MenuPicture>(filename, coordinates);
    auto item = std::make_unique<MenuButton>(
        std::move(pauseSprite), coordinates, rectangleSize,
        sf::Color(0, 0, 0, 0), sf::Color(128, 128, 128, 128),
        type);
    addMenuItem(std::move(item));
}

void Menu::addIconToLeftLowerCorner(const std::string &filename, ButtonType type) {
    sf::Image exitImage;
    exitImage.loadFromFile(filename);
    const int margin = 5;
    sf::Vector2<float> coordinates(
        margin,
        static_cast<float>(WINDOW_HEIGHT - exitImage.getSize().y - 3 * margin));
    sf::Vector2<float> rectangleSize(
        static_cast<float>(exitImage.getSize().x + 2 * margin),
        static_cast<float>(exitImage.getSize().y + 2 * margin));
    auto picture =
        std::make_unique<MenuPicture>(filename, coordinates);
    auto item = std::make_unique<MenuButton>(
        std::move(picture), coordinates, rectangleSize, sf::Color(0, 0, 0, 0),
        sf::Color(128, 128, 128, 128), type);
    addMenuItem(std::move(item));
}

void Menu::addMenuItem(std::unique_ptr<MenuItem> &&item) {
    items.emplace_back(std::move(item));
}

const std::vector<std::unique_ptr<MenuItem>> &Menu::getItems() const {
    return items;
}

MenuInscription::MenuInscription(const InscriptionInfo &parameters,
                                 const sf::Vector2<float> &coordinates) {
    font.loadFromFile("../fonts/base_bold.ttf");
    text.setFillColor(parameters.textColor);
    text.setFont(font);
    text.setCharacterSize(parameters.characterSize);
    text.setString(parameters.inscription);
    text.setPosition(coordinates);
    text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);
}

void MenuInscription::draw(sf::RenderWindow &window) const {
    window.draw(text);
}

sf::Vector2<float> MenuInscription::getSize() const {
    auto res = text.getGlobalBounds();
    return sf::Vector2<float>{res.width, res.height};
}

sf::Vector2<float> MenuInscription::getPosition() const {
    return text.getPosition();
}

void MenuInscription::setPosition(sf::Vector2<float> newPosition) {
    text.setPosition(newPosition.x, newPosition.y);  // fix bugs
}

MenuPicture::MenuPicture(const std::string &path,
                         const sf::Vector2<float> &coordinates) {
    image.loadFromFile(path);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setPosition(coordinates);
}

sf::Vector2<float> MenuPicture::getPosition() const {
    return sprite.getPosition();
}

sf::Vector2<float> MenuPicture::getSize() const {
    return static_cast<sf::Vector2<float>>(image.getSize());
}

void MenuPicture::setPosition(sf::Vector2<float> newPosition) {
    sprite.setPosition(newPosition);
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
    content->setPosition(
        {static_cast<float>(
             static_cast<int>(coordinates.x) +
             static_cast<int>((rectangleSize.x - content->getSize().x) / 2)),
         static_cast<float>(
             static_cast<int>(coordinates.y) +
             static_cast<int>((rectangleSize.y - content->getSize().y) / 2))});
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

sf::Vector2<float> MenuButton::getPosition() const {
    return rectangle.getPosition();
}

sf::Vector2<float> MenuButton::getSize() const {
    return rectangle.getSize();
}

void MenuButton::setPosition(sf::Vector2<float> newPosition) {
    rectangle.setPosition(newPosition);
}

}  // namespace Tanks::Menu