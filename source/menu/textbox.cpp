#include "menu/textbox.h"
#include <cassert>
#include <iostream>

namespace Tanks::Menu {
constexpr static int DELETE_KEY = 8;

TextBox::TextBox(size_t limit_,
                 const InscriptionInfo &info,
                 const sf::Vector2<float> &rectangleSize,  // NOLINT
                 const sf::Vector2<float> &pos,
                 const sf::Color &rectangleColor)
    : rectangle(rectangleSize),
      text(info.inscription),
      content(std::make_unique<MenuInscription>(
          InscriptionInfo{"user1", info.characterSize, info.textColor},
          pos)),
      limit(limit_),
      defaultColor(info.textColor) {
    setPosition(pos);
    content->setStandardPosition(content->getPosition());
    rectangle.setFillColor(rectangleColor);
}

sf::Vector2<float> TextBox::getSize() const {
    return rectangle.getSize();
}

sf::Vector2<float> TextBox::getPosition() const {
    return rectangle.getPosition();
}

std::string TextBox::getContent() const {
    return text;
}

void TextBox::setPosition(sf::Vector2<float> newPosition) {
    const static int marginLeft = 35;
    rectangle.setPosition(newPosition);
    content->setPosition(sf::Vector2<float>(newPosition.x + marginLeft, 0));
    content->centralizeByHeight({newPosition.y, newPosition.y + getSize().y});
}

void TextBox::draw(sf::RenderWindow &window) const {
    window.draw(rectangle);
    if (text.empty()) {
        const static sf::Color greyColor{128, 128, 128, 128};
        content->setContent("user1");
        content->setTextColor(greyColor);
        content->draw(window);
        content->setTextColor(defaultColor);
        content->setContent(text);
        content->setPosition(content->getStandardPosition());
    } else {
        content->setContent(text + (text.size() == limit ? "" : "_"));
        content->draw(window);
    }
}

void TextBox::input(char symbol) {
    if (symbol != DELETE_KEY) {
        if (text.size() < limit) {
            text += symbol;
        }
    } else {
        if (!text.empty()) {
            deleteLastCharacter();
        }
    }
}

void TextBox::deleteLastCharacter() {
    assert(!text.empty());
    text.pop_back();
}

}  // namespace Tanks::Menu