#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <sstream>
#include "menu.h"

namespace Tanks::Menu {

struct TextBox final : MenuItem {
public:
    TextBox(size_t limit_,
            const InscriptionInfo &info,
            const sf::Vector2<float> &rectangleSize,
            const sf::Vector2<float> &pos,
            const sf::Color &rectangleColor);

    sf::Vector2<float> getPosition() const final;
    sf::Vector2<float> getSize() const final;
    std::string getContent() const;

    void setPosition(sf::Vector2<float> newPosition) final;

    void draw(sf::RenderWindow &window) const final;

private:
    sf::RectangleShape rectangle;
    std::string text;
    mutable std::unique_ptr<MenuInscription> content;
    const size_t limit;
    const sf::Color defaultColor;

    void input(char symbol);

    void deleteLastCharacter();

    friend MenuController;
};

}  // namespace Tanks::Menu

#endif
