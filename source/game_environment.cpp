#include "game_environment.h"
#include <cassert>
#include "constants.h"
#include "controller.h"

namespace Tanks {

namespace {

std::string numberWithLeftZeros(int num, int numberLength) {
    std::string numStr(std::to_string(num));
    assert(numStr.size() <= numberLength);
    return std::string("0", numberLength - numStr.size()) + numStr;
}
}  // namespace

Timer::Timer(const std::string &path) {
    image.loadFromFile(path);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    const int marginRight = MARGIN_LEFT / 4;
    const int marginTop = MARGIN_TOP + TILE_SIZE;
    sprite.setPosition(
        static_cast<float>(MARGIN_LEFT - image.getSize().x - marginRight),
        marginTop);

    // text
    font.loadFromFile("../fonts/base.ttf");

    time.setFont(font);
    time.setString("00:00");
    time.setCharacterSize(28);
    time.setPosition(sprite.getPosition().x + (static_cast<float>(image.getSize().x) - time.getGlobalBounds().width) / 2, sprite.getPosition().y + static_cast<float>(image.getSize().y));
    restart();
}

void Timer::drawTimer(sf::RenderWindow &window) const {
    window.draw(sprite);
    window.draw(time);
}

void Timer::restart() {
    timer.restart();
}

void Timer::nextTick() {
    int different = static_cast<int>(timer.getElapsedTime().asSeconds());
    passedTime += different;
    if (different > 0) {
        restart();
    }
    int minutes = passedTime / 60;
    int seconds = passedTime % 60;
    time.setString(numberWithLeftZeros(minutes, 2) + ':' +
                   numberWithLeftZeros(seconds, 2));
    if (passedTime % 10 == 0) { // every 10 seconds centralize time
        time.setPosition(
                sprite.getPosition().x + (static_cast<float>(image.getSize().x) - time.getGlobalBounds().width) / 2,
                sprite.getPosition().y + static_cast<float>(image.getSize().y));
    }
}

Environment::Environment(const std::string &path) : timer(path + "timer.png"), menu(), background(initBackground(path)) {
    addPauseButton(path);
}

void Environment::draw(sf::RenderWindow &window, bool isPause) const {
    window.draw(background);
    if (!isPause) {
        timer.nextTick();
        menu.drawMenu(window);
    } else {
        timer.restart();
    }
    timer.drawTimer(window);
}

const Menu::Menu &Environment::getMenu() const {
    return menu;
}

void Environment::addPauseButton(const std::string &path) {
    static const std::string imageFilename = path + "pause.png";
    sf::Image image;
    image.loadFromFile(imageFilename);
    const int margin = 5;
    const sf::Vector2<float> coordinates{2*margin, 2*margin};
    const sf::Vector2<float> rectangleSize{static_cast<float>(image.getSize().x) + margin, static_cast<float>(image.getSize().y) + margin};
    auto item = std::make_unique<Menu::MenuButton>(imageFilename, coordinates, rectangleSize, sf::Color(0, 0, 0, 0), sf::Color(128, 128, 128, 128), Menu::ButtonType::PAUSE);
    menu.addMenuItem(std::move(item));
}

sf::Sprite Environment::initBackground(const std::string &path) {
    static const std::string backgroundImageFilename =
        path + "background.png";
    sf::Image backgroundImage;
    backgroundImage.loadFromFile(backgroundImageFilename);
    static sf::Texture backgroundTexture;  // so that the texture isn't
                                           // destroyed after the function exits
    backgroundTexture.loadFromImage(backgroundImage);
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);
    return backgroundSprite;
}

}  // namespace Tanks