#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include "menu.h"

namespace Tanks::Menu {

struct ProgressBarItem final : MenuItem {
public:
    ProgressBarItem(sf::Vector2<float> size, sf::Vector2<float> coordinates);

    sf::Vector2<float> getSize() const final;
    sf::Vector2<float> getPosition() const final;

    void setPosition(sf::Vector2<float> newPosition) final;

    void activate();

    void draw(sf::RenderWindow &window) const final;

private:
    sf::RectangleShape indicator;
};

enum class ProgressBarType { TANK_SPEED, BULLET_SPEED, RELOAD_TICKS, LIFE_AMOUNT };

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