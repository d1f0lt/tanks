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
    void setSliderVolume(float newValue);
    size_t getMaxVolume() const;
    SliderBarType getType() const;
    void logic(sf::RenderWindow &window);
    void draw(sf::RenderWindow &window) const final;
private:
    sf::RectangleShape slider;
    sf::RectangleShape axis;
    const size_t maxVolume;
    const size_t minVolume = 0;
    size_t curVolume = 50;
    std::unique_ptr<MenuInscription> title;
    const static size_t axisWidth = 10;
    const static size_t axisHeight = 200;
    const static size_t sliderWidth = 10;
    const static size_t sliderHeight = 20;
    SliderBarType type;
};

struct ProgressBar final : MenuItem {
public:
    ProgressBar(InscriptionInfo &titleInfo,
                size_t maxAmount_,
                size_t curProgress_,
                sf::Vector2<float> sizeOfOne,
                sf::Vector2<float> coordinates,
                ProgressBarType type_);

    sf::Vector2<float> getSize() const final;
    sf::Vector2<float> getPosition() const final;
    size_t getCurrentProgress() const;
    size_t getMaxProgress() const;
    sf::Vector2<float> getSizeOfOne() const;
    ProgressBarType getType() const;

    void setPosition(sf::Vector2<float> newPosition) final;

    [[maybe_unused]] void upgrade();

    void addInfoToRightUpCorner(
        std::vector<std::unique_ptr<MenuItem>> &&newItems);

    void draw(sf::RenderWindow &window) const final;

private:
    sf::RectangleShape wrapper;
    const size_t maxProgress;
    size_t curProgress = 0;
    std::vector<std::unique_ptr<ProgressBarItem>> items;
    std::unique_ptr<MenuInscription> title;
    std::vector<std::unique_ptr<MenuItem>> additionalElements;
    ProgressBarType type;

    const static size_t margin = 10;
};

}  // namespace Tanks::Menu

#endif