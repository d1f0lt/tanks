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

void addPauseButton(Menu::Menu &menu, const std::string &path) {
    static const std::string imageFilename = path + "pause.png";
    sf::Image image;
    image.loadFromFile(imageFilename);
    const int margin = 5;
    const sf::Vector2<float> coordinates{2 * margin, 2 * margin};
    const sf::Vector2<float> rectangleSize{
        static_cast<float>(image.getSize().x) + margin,
        static_cast<float>(image.getSize().y) + margin};
    auto pauseSprite =
        std::make_unique<Menu::MenuPicture>(imageFilename, coordinates);
    auto item = std::make_unique<Menu::MenuButton>(
        std::move(pauseSprite), coordinates, rectangleSize,
        sf::Color(0, 0, 0, 0), sf::Color(128, 128, 128, 128),
        Menu::ButtonType::PAUSE);
    menu.addMenuItem(std::move(item));
}

}  // namespace

Timer::Timer(const std::string &filename) {
    image.loadFromFile(filename);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    const static int marginRight = MARGIN_LEFT / 4;
    const static int marginTop = MARGIN_TOP + TILE_SIZE;
    const static int marginFromSprite = 100;
    sprite.setPosition(
        static_cast<float>(MARGIN_LEFT - image.getSize().x - marginRight),
        marginTop);

    // text
    font.loadFromFile("../fonts/base_bold.ttf");

    time.setFont(font);
    time.setString("00:00");
    time.setCharacterSize(28);
    time.setPosition(
        sprite.getPosition().x + (static_cast<float>(image.getSize().x) -
                                  time.getLocalBounds().width) /
                                     2,
        sprite.getPosition().y + static_cast<float>(image.getSize().y) +
            marginFromSprite);
    restart();
}

void Timer::draw(sf::RenderWindow &window) const {
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
    if (passedTime % 10 == 0) {  // every 10 seconds centralize time
        time.setPosition(
            sprite.getPosition().x + (static_cast<float>(image.getSize().x) -
                                      time.getGlobalBounds().width) /
                                         2,
            sprite.getPosition().y + static_cast<float>(image.getSize().y));
    }
}

Environment::Environment(const std::string &path)
    : timer(path + "timer.png"), menu() {
    addPauseButton(menu, path);
}

void Environment::draw(sf::RenderWindow &window, Pause &pause) const {
    if (!pause.isPause()) {
        timer.nextTick();
        menu.draw(window);
    } else {
        timer.restart();
    }
    timer.draw(window);
}

const Menu::Menu &Environment::getMenu() const {
    return menu;
}

}  // namespace Tanks