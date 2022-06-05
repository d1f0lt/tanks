#include "game_environment.h"
#include <cassert>
#include "constants.h"
#include "menu/menu.h"

namespace Tanks {

namespace {

std::string numberWithLeftZeros(int num, size_t numberLength) {  // NOLINT
    std::string numStr(std::to_string(num));
    assert(numStr.size() <= static_cast<size_t>(numberLength));
    return std::string("0", numberLength - numStr.size()) + numStr;
}

sf::Sprite initBackground(const std::string &path) {
    static const std::string backgroundImageFilename = path + "background.png";
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

Lives::Lives(const std::string &filename,
             size_t heartSize, // NOLINT
             size_t livesCount,
             size_t mxLivesCount)
    : sprites(mxLivesCount), livesAmount(mxLivesCount), sizeOfOne(heartSize) {
    image.loadFromFile(filename);
    texture.loadFromImage(image);
    for (auto &sprite : sprites) {
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect<int>(0, 0, static_cast<int>(sizeOfOne),
                                            static_cast<int>(sizeOfOne)));
    }
    while (livesCount != livesAmount) {
        destroyLastHeart();
    }
    setPosition();
}

void Lives::destroyLastHeart() {
    assert(livesAmount > 0);
    sprites[--livesAmount].setTextureRect(sf::Rect<int>(static_cast<int>(sizeOfOne), 0, static_cast<int>(sizeOfOne), static_cast<int>(sizeOfOne)));
}

void Lives::setPosition() {
    const static size_t marginTop = 100;
    const static size_t marginBetweenHearts = 10;
    const static float marginLeft = static_cast<float>(WINDOW_WIDTH - MARGIN_LEFT) + static_cast<float>(MARGIN_LEFT - sizeOfOne * 2 - marginBetweenHearts) / 2;
    sf::Vector2<float> curCoordinates{static_cast<float>(marginLeft), static_cast<float>(marginTop)};
    for (size_t i = 0; i < sprites.size(); ++i) {
        sprites[i].setPosition(curCoordinates);
        if (i % 2 == 0) {
            curCoordinates.x += static_cast<float>(marginBetweenHearts + sizeOfOne);
        } else {
            curCoordinates.x = static_cast<float>(marginLeft);
            curCoordinates.y += static_cast<float>(marginBetweenHearts + sizeOfOne);
        }
    }
    if (sprites.size() % 2 == 1) {
        assert(MARGIN_LEFT > sizeOfOne);
        curCoordinates.x = (marginLeft + static_cast<float>(sizeOfOne + marginBetweenHearts) / 2);
        sprites.back().setPosition(curCoordinates);
    }
}

size_t Lives::getLivesAmount() const {
    return livesAmount
}

void Lives::draw(sf::RenderWindow &window) const {
    for (const auto &sprite : sprites) {
        window.draw(sprite);
    }
}

static const size_t heartSize = 72;

Environment::Environment(const std::string &path, size_t lifeAmount)
    : lives(path + "hearts.png", heartSize, lifeAmount, MAX_LIVES_AMOUNT),
      timer(path + "timer.png"),
      backgroundSprite(initBackground(path)) {
    menu.addIconToLeftUpCorner(path + "pause.png", Menu::ButtonType::PAUSE);
}

void Environment::destroyLastHeart() {
    lives.destroyLastHeart();
}

size_t Environment::getLivesAmount() const {
    return lives.getLivesAmount();
}

void Environment::draw(sf::RenderWindow &window, bool isPause) const {
    window.draw(backgroundSprite);
    if (!isPause) {
        timer.nextTick();
        menu.draw(window);
    } else {
        timer.restart();
    }
    timer.draw(window);
    lives.draw(window);
}

const Menu::Menu &Environment::getMenu() const {
    return menu;
}

}  // namespace Tanks