#include "menu/menu_items.h"
#include <cassert>
#include <unordered_map>
#include "constants.h"

namespace Tanks::Menu {

std::string convertButtonTypeToString(const ButtonType type) {
    static std::unordered_map<ButtonType, std::string> dictionary{
        {ButtonType::NEW_GAME, "NEW GAME"},
        {ButtonType::EXIT, "EXIT"},
        {ButtonType::RESUME, "RESUME"},
        {ButtonType::SETTINGS, "SETTINGS"},
        {ButtonType::RATING, "RATING"},
        {ButtonType::PAUSE, "PAUSE"},
        {ButtonType::UPGRADE, "UPGRADE"},
        {ButtonType::SINGLE_PLAYER, "SINGLE PLAYER"},
        {ButtonType::MULTIPLAYER, "MULTIPLAYER"}};
    assert(dictionary.find(type) != dictionary.end());
    return dictionary[type];
}

Button::Button(const sf::Vector2<float> &size_,
               const sf::Color &standardColor_,  // NOLINT
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
    setTextColor(parameters.textColor);
    text.setFont(font);
    text.setCharacterSize(parameters.characterSize);
    setContent(parameters.inscription);
    updateOrigin();
    setPosition(coordinates);
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

void MenuInscription::updateOrigin() {
    text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);
}

void MenuInscription::setPosition(sf::Vector2<float> newPosition) {
    text.setPosition(newPosition.x, newPosition.y);
}

void MenuInscription::setContent(const std::string &newString) {
    text.setString(newString);
}

void MenuInscription::setTextColor(const sf::Color &color) {
    text.setFillColor(color);
}

std::string MenuInscription::getContent() const {
    return static_cast<std::string>(text.getString());
}

MenuRectangle::MenuRectangle(Button &info, const sf::Vector2<float> &coordinates) : rectangle(info.getSize()) {
    rectangle.setPosition(coordinates);
    rectangle.setFillColor(info.getStandardColor());
}

sf::Vector2<float> MenuRectangle::getSize() const {
    return rectangle.getSize();
}

sf::Vector2<float> MenuRectangle::getPosition() const {
    return rectangle.getPosition();
}

void MenuRectangle::setPosition(sf::Vector2<float> newPosition) {
    rectangle.setPosition(newPosition);
}

void MenuRectangle::draw(sf::RenderWindow &window) const {
    window.draw(rectangle);
}

MenuButton::MenuButton(std::unique_ptr<MenuItem> &&content_,
                       const sf::Vector2<float> &coordinates,
                       ButtonWithType info_)
    : MenuRectangle(info_, coordinates),
      content(std::move(content_)),
      info(info_) {
    rectangle.setFillColor(info.getStandardColor());
    setPosition(coordinates);  // NOLINT
}

void MenuButton::draw(sf::RenderWindow &window) const {
    window.draw(rectangle);
    content->draw(window);
    rectangle.setFillColor(
        info.getStandardColor());  // recover after possible hover;
}

void MenuButton::hover() {
    rectangle.setFillColor(info.getHoverColor());
}

ButtonType MenuButton::getType() const {
    return info.getType();
}

sf::Vector2<float> MenuButton::getPosition() const {
    return rectangle.getPosition();
}

sf::Vector2<float> MenuButton::getSize() const {
    return rectangle.getSize();
}

std::string MenuButton::getInscription() const {
    auto *inscription = dynamic_cast<MenuInscription *>(content.get());
    assert(inscription != nullptr);
    return inscription->getContent();
}

void MenuButton::setPosition(sf::Vector2<float> newPosition) {
    rectangle.setPosition(newPosition);
    content->centralizeByWidth(
        {getPosition().x, getPosition().x + getSize().x});
    content->centralizeByHeight(
        {getPosition().y, getPosition().y + getSize().y});
}

MenuItem *MenuButton::getContent() const {
    return content.get();
}

sf::RectangleShape &MenuButton::getRectangle() const {
    return rectangle;
}

const ButtonWithType &MenuButton::getButtonInfo() const {
    return info;
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

MenuPicture::MenuPicture(const std::string &filename,  size_t sizeOfOne, size_t count, const sf::Vector2<float> &coordinates) : MenuItem(coordinates) { // NOLINT
    image.loadFromFile(filename);
    texture.loadFromImage(image);
    sprites.resize(count);
    for (size_t i = 0; i < count; ++i) {
        sprites[i].setTexture(texture);
        sprites[i].setTextureRect(sf::IntRect{static_cast<int>(sizeOfOne * i), 0, static_cast<int>(sizeOfOne), static_cast<int>(sizeOfOne)});
        sprites[i].setPosition(coordinates);
    }
}

void MenuPicture::initWithImage(const sf::Vector2<float> &coordinates) {
    texture.loadFromImage(image);
    sprites.resize(1);
    sprites[0].setTexture(texture);
    sprites[0].setPosition(coordinates);
}

sf::Vector2<float> MenuPicture::getPosition() const {
    assert(!sprites.empty());
    return sprites[0].getPosition();
}

sf::Vector2<float> MenuPicture::getSize() const {
    assert(!sprites.empty());
    return sf::Vector2<float>{sprites[0].getLocalBounds().width, sprites[0].getLocalBounds().height};
}

void MenuPicture::setPosition(sf::Vector2<float> newPosition) {
    for (auto &sprite : sprites) {
        sprite.setPosition(newPosition);
    }
}

void MenuPicture::draw(sf::RenderWindow &window) const {
    window.draw(sprites[curIndex]);
    if (timer.getElapsedTime().asMilliseconds() > 100) {
        timer.restart();
        curIndex = (curIndex + 1) % sprites.size();
    }
}

MenuPictureWithDescription::MenuPictureWithDescription(
    std::unique_ptr<MenuPicture> &&picture_,
    std::unique_ptr<MenuInscription> &&description_,
    const sf::Vector2<float> &coordinates,
    Button btnInfo)
    : MenuButton(std::move(picture_),
                 coordinates,
                 ButtonWithType(btnInfo, ButtonType::LEVEL)),
      description(std::move(description_)) {
    const auto *picture = getContent();
    assert(picture->getSize().x >= description->getSize().x);
    getRectangle().setSize(sf::Vector2<float>(
        picture->getSize().x,
        picture->getSize().y + 2 * TEXT_MARGIN + description->getSize().y));
    setPosition(coordinates);
}

void MenuPictureWithDescription::draw(sf::RenderWindow &window) const {
    window.draw(getRectangle());
    getContent()->draw(window);
    description->draw(window);
    getRectangle().setFillColor(
        getButtonInfo().getStandardColor());  // recover after possible hover;
}

sf::Vector2<float> MenuPictureWithDescription::getSize() const {
    return getRectangle().getSize();
}

sf::Vector2<float> MenuPictureWithDescription::getPosition() const {
    return getRectangle().getPosition();
}

void MenuPictureWithDescription::setPosition(sf::Vector2<float> newPosition) {
    auto &rectangle = getRectangle();
    auto *picture = getContent();
    rectangle.setPosition(newPosition);
    getContent()->setPosition(getPosition());
    description->centralizeByHeight(
        {rectangle.getPosition().y + picture->getSize().y,
         rectangle.getPosition().y + rectangle.getSize().y});
    description->centralizeByWidth(
        {getPosition().x, newPosition.x + getSize().x});
}

std::string MenuPictureWithDescription::getDescription() const {
    return description->getContent();
}

MenuAdditionalButton::MenuAdditionalButton(const MenuItem *const mainButton_,
                                           int marginFromLeft,
                                           std::unique_ptr<MenuItem> &&content,
                                           ButtonWithType &info)
    : MenuButton(std::move(content),
                 sf::Vector2<float>{mainButton_->getPosition().x +
                                        static_cast<float>(marginFromLeft) +
                                        mainButton_->getSize().x,
                                    mainButton_->getPosition().y},
                 info),
      mainButton(mainButton_) {
}

const MenuItem *MenuAdditionalButton::getMainButton() const {
    return mainButton;
}

}  // namespace Tanks::Menu