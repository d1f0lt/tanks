#include "menu_items.h"
#include <cassert>
#include "constants.h"

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

Button::Button(const sf::Vector2<float> &size_,
               const sf::Color &standardColor_,
               const sf::Color &hoverColor_)
    : standardColor(standardColor_), hoverColor(hoverColor_), size(size_) {
}

const sf::Vector2<float> &Button::getSize() const {
    return size;
}

const sf::Color &Button::getHoverColor() const {
    return hoverColor;
}

const sf::Color &Button::getStandardColor() const {
    return standardColor;
}

void Button::setSize(const sf::Vector2<float> &newSize) {
    size = newSize;
}

ButtonWithType::ButtonWithType(ButtonType type_,
                               const sf::Vector2<float> &size_,
                               const sf::Color &standardColor_,
                               const sf::Color &hoverColor_)
    : Button(size_, standardColor_, hoverColor_), type(type_) {
}

ButtonWithType::ButtonWithType(Button btn, ButtonType type_)
    : Button(btn), type(type_) {
}

ButtonType ButtonWithType::getType() const {
    return type;
}

MenuItem::MenuItem(const sf::Vector2<float> &coordinates)
    : standardCoordinates(coordinates) {
}

const sf::Vector2<float> &MenuItem::getStandardPosition() const {
    return standardCoordinates;
}

void MenuItem::setStandardPosition(const sf::Vector2<float> &newPosition) {
    standardCoordinates = newPosition;
}

void MenuItem::move(const sf::Vector2<float> &distance) {
    setPosition(sf::Vector2<float>(getPosition().x + distance.x,
                                   getPosition().y + distance.y));
}

void MenuItem::centralizeByHeight(
    const std::pair<float, float> &rectangleCoordinatesY) {
    assert(rectangleCoordinatesY.first < rectangleCoordinatesY.second);
    assert(rectangleCoordinatesY.second - rectangleCoordinatesY.first >=
           getSize().y);
    setPosition(sf::Vector2<float>(
        getPosition().x,
        static_cast<float>(
            static_cast<int>(rectangleCoordinatesY.first) +
            static_cast<int>((rectangleCoordinatesY.second -
                              rectangleCoordinatesY.first - getSize().y) /
                             2))));
}

void MenuItem::centralizeByWidth(
    const std::pair<float, float> &rectangleCoordinatesX) {
    assert(rectangleCoordinatesX.first < rectangleCoordinatesX.second);
    assert(rectangleCoordinatesX.second - rectangleCoordinatesX.first >=
           getSize().x);
    setPosition(sf::Vector2<float>(
        static_cast<float>(
            static_cast<int>(rectangleCoordinatesX.first) +
            static_cast<int>((rectangleCoordinatesX.second -
                              rectangleCoordinatesX.first - getSize().x) /
                             2)),
        getPosition().y));
}

MenuInscription::MenuInscription(const InscriptionInfo &parameters,
                                 const sf::Vector2<float> &coordinates)
    : MenuItem(coordinates) {
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

std::string MenuInscription::getContent() const {
    return std::string(text.getString());
}

MenuButton::MenuButton(std::unique_ptr<MenuItem> &&content_,
                       const sf::Vector2<float> &coordinates,
                       ButtonWithType info_)
    : MenuItem(coordinates),
      content(std::move(content_)),
      info(info_),
      rectangle(info_.getSize()) {
    rectangle.setFillColor(info.getStandardColor());
    setPosition(coordinates);  // NOLINT
}

void MenuButton::draw(sf::RenderWindow &window) const {
    window.draw(rectangle);
    content->draw(window);
    rectangle.setFillColor(
        info.getStandardColor());  // recover after possible hover;
}

ButtonType MenuButton::getType() const {
    return info.getType();
}

void MenuButton::hover() {
    rectangle.setFillColor(info.getHoverColor());
}

sf::Vector2<float> MenuButton::getPosition() const {
    return rectangle.getPosition();
}

sf::Vector2<float> MenuButton::getSize() const {
    return rectangle.getSize();
}

void MenuButton::setPosition(sf::Vector2<float> newPosition) {
    rectangle.setPosition(newPosition);
    content->centralizeByWidth(
        {getPosition().x, getPosition().x + getSize().x});
    content->centralizeByHeight(
        {getPosition().y, getPosition().y + getSize().y});
}

MenuPicture::MenuPicture(const std::string &filename,
                         const sf::Vector2<float> &coordinates)
    : MenuItem(coordinates) {
    image.loadFromFile(filename);
    initWithImage(coordinates);
}

MenuPicture::MenuPicture(const sf::Image &image_,
                         const sf::Vector2<float> &coordinates)
    : MenuItem(coordinates), image(image_) {
    initWithImage(coordinates);
}

void MenuPicture::initWithImage(const sf::Vector2<float> &coordinates) {
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

#define picture content

MenuPictureWithDescription::MenuPictureWithDescription(
    std::unique_ptr<MenuPicture> &&picture_,
    std::unique_ptr<MenuInscription> &&description_,
    const sf::Vector2<float> &coordinates,
    Button btnInfo)
    : MenuButton(std::move(picture_),
                 coordinates,
                 ButtonWithType(btnInfo, ButtonType::LEVEL)),
      description(std::move(description_)) {
    assert(picture->getSize().x >= description->getSize().x);
    rectangle.setSize(sf::Vector2<float>(
        picture->getSize().x,
        picture->getSize().y + 2 * TEXT_MARGIN + description->getSize().y));
    setPosition(coordinates);
}

void MenuPictureWithDescription::draw(sf::RenderWindow &window) const {
    window.draw(rectangle);
    picture->draw(window);
    description->draw(window);
    rectangle.setFillColor(
        info.getStandardColor());  // recover after possible hover;
}

sf::Vector2<float> MenuPictureWithDescription::getSize() const {
    return rectangle.getSize();
}

sf::Vector2<float> MenuPictureWithDescription::getPosition() const {
    return rectangle.getPosition();
}

void MenuPictureWithDescription::setPosition(sf::Vector2<float> newPosition) {
    rectangle.setPosition(newPosition);
    picture->setPosition(getPosition());
    description->centralizeByHeight(
        {rectangle.getPosition().y + picture->getSize().y,
         rectangle.getPosition().y + rectangle.getSize().y});
    description->centralizeByWidth(
        {getPosition().x, newPosition.x + getSize().x});
}

std::string MenuPictureWithDescription::getDescription() const {
    return description->getContent();
}

}  // namespace Tanks::Menu