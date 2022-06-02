#ifndef SLIDER_BAR_H
#define SLIDER_BAR_H

#include "menu.h"

namespace Tanks::Menu {

enum class SliderBarType {SOUND, MUSIC };


struct SliderBar final : MenuItem {
public:
    SliderBar(InscriptionInfo &titleInfo,
              sf::Vector2<float> coordinates_,
              size_t maxValue_,
              size_t curValue_,
              SliderBarType type_);
    sf::Vector2<float> getSize() const final;
    sf::Vector2<float> getPosition() const final;
    size_t getCurrentVolume() const;
    void setSliderVolume(size_t newValue);
    void setPosition(sf::Vector2<float> newPosition) final;
    size_t getMaxVolume() const;
    SliderBarType getType() const;
    void logic(sf::RenderWindow &window);
    void draw(sf::RenderWindow &window) const final;
private:
    sf::RectangleShape slider;
    sf::RectangleShape axis;
    sf::Vector2<float> coordinates;
    const size_t maxVolume;
    const size_t minVolume = 0;
    size_t curVolume = 50;
    std::unique_ptr<MenuInscription> title;
    const static size_t axisWidth = 200;
    const static size_t axisHeight = 10;
    const static size_t sliderWidth = 10;
    const static size_t sliderHeight = 20;
    SliderBarType type;
};

}  // namespace Tanks::Menu

#endif