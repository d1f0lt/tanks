#ifndef INPUT_MENU_H
#define INPUT_MENU_H

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

namespace Tanks::Menu {

std::optional<std::string> showInputMenu(sf::RenderWindow &window,
                   const sf::Sprite &background,
                   const std::string &title);

}

#endif