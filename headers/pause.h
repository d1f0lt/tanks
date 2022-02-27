#ifndef PAUSE_H
#define PAUSE_H

#include <SFML/Graphics.hpp>
#include "constants.h"
#include <vector>
#include <memory>

namespace Tanks {

struct PauseItem;

struct Pause final {
public:

    Pause();

    void drawPause(sf::RenderWindow &window) const;

    [[nodiscard]] bool isPause() const;

    void checkPause(const sf::Event &event);

private:
    bool pause = false;
    sf::RectangleShape background;
    std::vector<std::unique_ptr<PauseItem>> items;
    const static int pauseWidth = 400;

    friend struct PauseButton;
};

struct PauseItem {
public:
    explicit PauseItem(const std::string &path, const sf::Vector2<float> &coordinates);

    virtual void drawSprite(sf::RenderWindow &window) const;

protected:
    sf::Image inscriptionImage;
    sf::Texture inscriptionTexture;
    sf::Sprite inscriptionSprite;

    friend struct Pause;
};

struct PauseButton final : PauseItem {
public:
    PauseButton(const std::string &path, const sf::Vector2<float> &coordinates);

    void drawSprite(sf::RenderWindow &window) const final;
private:
    sf::RectangleShape rectangle;
    const static int rectangleHeight = 80;
    const static int rectangleWidth = Pause::pauseWidth;

    friend struct Pause;
};

}

#endif