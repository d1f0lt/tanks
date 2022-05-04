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

const static int textMargin = 10;

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
            static_cast<float>(image.getSize().y + 2 * textMargin) +
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
        maxPositionX = (pos > maxPositionX ? pos : maxPositionX);
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
        minPositionX = (pos < minPositionX ? pos : minPositionX);
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
        minPositionX = (pos < minPositionX ? pos : minPositionX);
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
        picture->getSize().y + 2 * textMargin + description->getSize().y));
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