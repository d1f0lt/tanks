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

    [[nodiscard]] const std::vector<std::unique_ptr<PauseItem>> &getItems() const;

    friend struct PauseButton;
    friend struct PauseController;
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
    friend struct PauseController;
};

struct PauseButton final : PauseItem {
public:
    PauseButton(const std::string &path, const sf::Vector2<float> &coordinates);

    void drawSprite(sf::RenderWindow &window) const final;

    void hover();

private:
    mutable sf::RectangleShape rectangle;
    const static int rectangleHeight = 80;
    const static int rectangleWidth = Pause::pauseWidth;

    friend struct Pause;
    friend struct PauseController;
};

}

#endif