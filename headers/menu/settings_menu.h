#ifndef SETTINGS_MENU_H
#define SETTINGS_MENU_H

#include "menu/menu.h"

namespace Tanks::Menu {

void showSettingsMenu(sf::RenderWindow &window,
                      const sf::Sprite &backgroundSprite,
                      PlayerInfo &info);

}

#endif