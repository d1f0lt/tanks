#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "menu/menu.h"

namespace Tanks {

std::optional<Menu::ButtonType> startGame(
    sf::RenderWindow &window,
    int level = 1,
    std::optional<std::pair<std::string, std::string>> address = std::nullopt);

}

#endif  // GAME_H_
