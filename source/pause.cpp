#include "pause.h"
#include <string>
#include "controller.h"

namespace Tanks {

Pause::Pause() : background(sf::Vector2<float>(WINDOW_WIDTH, WINDOW_HEIGHT)) {
    background.setPosition(0, 0);
    background.setFillColor(sf::Color(0, 0, 0, 180));

    const std::string path = "../images/pause/";
    const int paddingBetweenButtons = 10;
    const int marginLeft = (WINDOW_WIDTH - pauseWidth) / 2;
    const int marginTop = 250;  // some calculate
    const int elementsCount = 5;
    sf::Vector2<float> currentCoordinates = {marginLeft, marginTop};

    // Header
    items.emplace_back(
        std::make_unique<PauseItem>(path + "pause.png", currentCoordinates));
    items[0]->inscriptionSprite.setPosition(
        {static_cast<float>(
             static_cast<int>(currentCoordinates.x) +
             static_cast<int>(
                 (pauseWidth - items[0]->inscriptionImage.getSize().x) / 2)),
         currentCoordinates.y});  // Centralize

    // Buttons
    currentCoordinates.y += paddingBetweenButtons * 12;
    for (int i = 2; i <= elementsCount; ++i) {
        items.emplace_back(std::make_unique<PauseButton>(
            path + "item_" + std::to_string(i) + ".png", currentCoordinates));
        currentCoordinates.y +=
            PauseButton::rectangleHeight + paddingBetweenButtons;
    }
}

void Pause::drawPause(sf::RenderWindow &window) const {
    window.draw(background);
    for (const auto &item : items) {
        item->drawSprite(window);
    }
}

void Pause::checkPause(const sf::Event &event) {
    pause ^= GameController::isEscReleased(event);
}

bool Pause::isPause() const {
    return pause;
}

const std::vector<std::unique_ptr<PauseItem>> &Pause::getItems() const {
    return items;
}

PauseItem::PauseItem(const std::string &path,
                     const sf::Vector2<float> &coordinates) {
    inscriptionImage.loadFromFile(path);
    inscriptionTexture.loadFromImage(inscriptionImage);
    inscriptionSprite.setTexture(inscriptionTexture);
}

void PauseItem::drawSprite(sf::RenderWindow &window) const {
    window.draw(inscriptionSprite);
}

PauseButton::PauseButton(const std::string &path,
                         const sf::Vector2<float> &coordinates)
    : PauseItem(path, coordinates),
      rectangle(sf::Vector2<float>(static_cast<float>(rectangleWidth),
                                   static_cast<float>(rectangleHeight))) {
    rectangle.setFillColor(sf::Color(0, 0, 0));
    rectangle.setPosition(coordinates);
    inscriptionSprite.setPosition(
        static_cast<float>(
            static_cast<int>(coordinates.x) +
            static_cast<int>((rectangleWidth - inscriptionImage.getSize().x) /
                             2)),
        static_cast<float>(
            static_cast<int>(coordinates.y) +
            static_cast<int>((rectangleHeight - inscriptionImage.getSize().y) /
                             2)));  // Centralize
}

void PauseButton::drawSprite(sf::RenderWindow &window) const {
    window.draw(rectangle);
    window.draw(inscriptionSprite);
    rectangle.setFillColor(
        sf::Color(0, 0, 0));  // recover after possible hover;
}

void PauseButton::hover() {
    rectangle.setFillColor(sf::Color(115, 115, 115));
}

}  // namespace Tanks