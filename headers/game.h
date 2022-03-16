#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>
#include "menu.h"

namespace Tanks {

std::optional<Menu::ButtonType> startGame(sf::RenderWindow &window,
                                          int level = 1);

}

#endif  // GAME_H_