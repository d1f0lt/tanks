#ifndef INPUT_MENU_H
#define INPUT_MENU_H

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

namespace Tanks::Menu {

enum class FlyDirection {LEFT, RIGHT};

std::optional<std::string> showInputMenu(sf::RenderWindow &window,
                                         const sf::Sprite &background,
                                         const std::string &title,
                                         const std::string &defaultValue,
                                         FlyDirection flyDirectionAfterInput = FlyDirection::RIGHT);

}

#endif
