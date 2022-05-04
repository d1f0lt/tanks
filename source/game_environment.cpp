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
    menu.addIconToLeftUpCorner(path + "pause.png", Menu::ButtonType::PAUSE);
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