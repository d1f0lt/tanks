#ifndef UPGRADE_MENU_H
#define UPGRADE_MENU_H

#include "menu.h"

namespace Tanks::Menu {

void showSettingsMenu(sf::RenderWindow &window,
                     const sf::Sprite &backgroundSprite,
                     PlayerInfo &info);

}

#endif