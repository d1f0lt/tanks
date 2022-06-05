#ifndef LEVELS_MENU_H
#define LEVELS_MENU_H

#include "menu/menu.h"

namespace Tanks::Menu {

ButtonType showLevelsMenu(sf::RenderWindow &window,
                          const sf::Sprite &background,
                          PlayerInfo &info,
                          int playersAmount = 1);

}

#endif