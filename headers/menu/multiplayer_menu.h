#ifndef MULTIPLAYER_MENU_H
#define MULTIPLAYER_MENU_H

#include "menu/menu.h"

namespace Tanks::Menu {

void showMultiplayerMenu(sf::RenderWindow &window,
                     const sf::Sprite &background,
                     PlayerInfo &info);

}

#endif