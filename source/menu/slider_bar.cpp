#include "menu/progress_bar.h"
#include <cassert>

namespace Tanks::Menu {


SliderBar::SliderBar(InscriptionInfo &titleInfo,
                     sf::Vector2<float> coordinates_,
                     size_t maxValue_,
                     size_t curValue_,
                     SliderBarType type_)
    : maxVolume(maxValue_),
      curVolume(curVolume_),
      title(std::make_unique<MenuInscription>(titleInfo, coordinates)),
      type(type_),
      coordinates(coordinates_) {
    slider.setPosition(coordinates_);
    axis.setPosition(coordinates_);
}

size_t SliderBar::getCurrentVolume() const {
    return curVolume;
}

size_t SliderBar::getMaxVolume() const {
    return maxVolume;
}

SliderBarType SliderBar::getType() const {
    return type;
}

void SliderSFML::logic(sf::RenderWindow &window) {
    if (slider.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)
        && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (sf::Mouse::getPosition(window).x >= coordinates.x && sf::Mouse::getPosition(window).x <= coordinates.x + axisWidth)
        {
            slider.setPosition(sf::Mouse::getPosition(window).x, yCord);
            curVolume = (minVolume+ ((slider.getPosition().x - coordinates.x) / axisWidth * (maxVolume - minVolume)));
        }
    }
}

void SliderBar::draw const final (sf::RenderWindow &window)
{
    logic(window);
    window.draw(axis);
    window.draw(slider);
}

}  // namespace Tanks::Menu 