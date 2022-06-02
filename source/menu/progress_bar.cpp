#include "menu/progress_bar.h"
#include <cassert>

namespace Tanks::Menu {

ProgressBarItem::ProgressBarItem(sf::Vector2<float> size,  // NOLINT
                                 sf::Vector2<float> coordinates)
    : indicator(size) {
    setPosition(coordinates);
    const static sf::Color defaultIndicatorColor{42, 48, 64};
    indicator.setFillColor(defaultIndicatorColor);
}

sf::Vector2<float> ProgressBarItem::getSize() const {
    return indicator.getSize();
}

sf::Vector2<float> ProgressBarItem::getPosition() const {
    return indicator.getPosition();
}

void ProgressBarItem::setPosition(sf::Vector2<float> newPosition) {
    indicator.setPosition(newPosition);
}

void ProgressBarItem::activate() {
    const static sf::Color activeIndicatorColor{90, 190, 56};
    assert(indicator.getFillColor() != activeIndicatorColor);
    indicator.setFillColor(activeIndicatorColor);
}

void ProgressBarItem::draw(sf::RenderWindow &window) const {
    window.draw(indicator);
}

ProgressBar::ProgressBar(InscriptionInfo &titleInfo,
                         size_t maxAmount,  // NOLINT
                         size_t curProgress_,
                         sf::Vector2<float> sizeOfOne,  // NOLINT
                         sf::Vector2<float> coordinates,
                         ProgressBarType type_)
    : maxProgress(maxAmount),
      curProgress(curProgress_),
      title(std::make_unique<MenuInscription>(titleInfo, coordinates)),
      type(type_) {
    items.reserve(maxProgress);
    for (size_t i = 0; i < maxProgress; ++i) {
        auto item = std::make_unique<ProgressBarItem>(sizeOfOne, coordinates);
        if (i < curProgress) {
            item->activate();
        }
        items.emplace_back(std::move(item));
    }
    wrapper.setSize(sf::Vector2<float>{
        static_cast<float>(maxProgress) * (sizeOfOne.x + margin) + margin,
        3 * margin + sizeOfOne.y + title->getSize().y});
    const static sf::Color wrapperColor{60, 66, 90};
    wrapper.setFillColor(wrapperColor);
    setPosition(coordinates);
    setStandardPosition(getPosition());
}

sf::Vector2<float> ProgressBar::getSize() const {
    return sf::Vector2<float>{wrapper.getSize().x,
                              wrapper.getSize().y - margin};
}

sf::Vector2<float> ProgressBar::getPosition() const {
    return wrapper.getPosition();
}

size_t ProgressBar::getCurrentProgress() const {
    return curProgress;
}

size_t ProgressBar::getMaxProgress() const {
    return maxProgress;
}

sf::Vector2<float> ProgressBar::getSizeOfOne() const {
    return items[0]->getSize();
}

ProgressBarType ProgressBar::getType() const {
    return type;
}

void ProgressBar::setPosition(sf::Vector2<float> newPosition) {
    wrapper.setPosition(newPosition);
    sf::Vector2<float> curCoordinates{newPosition.x + margin,
                                      newPosition.y + margin};
    title->setPosition(curCoordinates);
    for (size_t i = 0; i < additionalElements.size(); ++i) {
        auto &item = additionalElements[i];
        if (i == 0) {
            item->setPosition(
                {newPosition.x + getSize().x - margin - item->getSize().x,
                 newPosition.y + margin});
        } else {
            item->setPosition({additionalElements[i - 1]->getPosition().x -
                                   item->getSize().x - margin,
                               additionalElements[i - 1]->getPosition().y});
        }
    }
    curCoordinates.y += title->getSize().y + margin;
    for (size_t i = 0; i < maxProgress; ++i) {
        items[i]->setPosition(curCoordinates);
        curCoordinates.x += items[i]->getSize().x + margin;
    }
}

void ProgressBar::upgrade() {
    assert(maxProgress > curProgress);
    items[curProgress++]->activate();
}

void ProgressBar::addInfoToRightUpCorner(
    std::vector<std::unique_ptr<MenuItem>> &&newItems) {
    for (auto &item : newItems) {
        additionalElements.emplace_back(std::move(item));
    }
    setPosition(getPosition());
}

void ProgressBar::draw(sf::RenderWindow &window) const {
    window.draw(wrapper);
    title->draw(window);
    for (const auto &item : additionalElements) {
        item->draw(window);
    }
    for (const auto &item : items) {
        item->draw(window);
    }
}

}  // namespace Tanks::Menu