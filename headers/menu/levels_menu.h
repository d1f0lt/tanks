#ifndef LEVELS_MENU_H
#define LEVELS_MENU_H

#include "menu/menu.h"
#include "menu.h"
#include "sound/background_music.h"

namespace Tanks::Menu {

ButtonType showLevelsMenu(sf::RenderWindow &window,
                          const sf::Sprite &background,
                          PlayerInfo &info,
                          Sound::BackgroundMusicHolder &backgroundMusicHolder,
                          int playersAmount = 1);

}

#endif