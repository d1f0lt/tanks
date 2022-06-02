#include "menu/slider_bar.h"
#include <cassert>

namespace Tanks::Menu {


SliderBar::SliderBar(InscriptionInfo &titleInfo,
                     sf::Vector2<float> coordinates_,
                     size_t maxValue_,
                     size_t curValue_,
                     SliderBarType type_)
    : maxVolume(maxValue_),
      curVolume(curValue_),
      title(std::make_unique<MenuInscription>(titleInfo, coordinates_)),
      type(type_),
      coordinates(coordinates_) {
    slider.setPosition(coordinates_);
    slider.setSize(sf::Vector2f(sliderWidth, sliderHeight));
    slider.setFillColor(sf::Color::Yellow);
    axis.setPosition(coordinates_);
    axis.setSize(sf::Vector2f(axisWidth, axisHeight));
    axis.setFillColor(sf::Color(230,0,230));
    setSliderVolume(curValue_);
}

sf::Vector2<float> SliderBar::getSize() const {
    return slider.getSize();
}

void SliderBar::setPosition(sf::Vector2<float> newPosition) {
    axis.setPosition(newPosition);
    coordinates = newPosition;
    sf::Vector2<float> curCoordinates{newPosition.x + sliderWidth,
                                      newPosition.y + sliderHeight};
    title->setPosition(curCoordinates);
    setSliderVolume(curVolume);
}

size_t SliderBar::getCurrentVolume() const {
    return curVolume;
}

sf::Vector2<float> SliderBar::getPosition() const {
    return slider.getPosition();
}

size_t SliderBar::getMaxVolume() const {
    return maxVolume;
}

SliderBarType SliderBar::getType() const {
    return type;
}

void SliderBar::setSliderVolume(size_t newVolume) {
    if (newVolume >= minVolume && newVolume <= maxVolume) {
        curVolume = newVolume;
        auto all_range = maxVolume - minVolume;
        auto cur_range = newVolume - minVolume;
        auto filled_part = axisWidth / all_range;
        auto cur_pos = filled_part * cur_range + coordinates.x;
        slider.setPosition(cur_pos, coordinates.y);
    }
}

void SliderBar::logic (sf::RenderWindow &window) {
    if (slider.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)
        && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (sf::Mouse::getPosition(window).x >= coordinates.x && sf::Mouse::getPosition(window).x <= coordinates.x + axisWidth) {
                slider.setPosition(sf::Mouse::getPosition(window).x, coordinates.y);
                float pos = slider.getPosition().x;
                curVolume = (minVolume+ ((pos - coordinates.x) / axisWidth * (maxVolume - minVolume)));
            }
    }
}

void SliderBar::draw (sf::RenderWindow &window) const {
//    logic(window);
    window.draw(axis);
    window.draw(slider);
}

}  // namespace Tanks::Menu 