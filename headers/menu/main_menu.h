#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "menu.h"
#include "sound/background_music.h"

namespace Tanks::Menu {

void showMainMenu(sf::RenderWindow &window,
                  const sf::Sprite &backgroundSprite,
                  PlayerInfo &info,
                  Sound::BackgroundMusicHolder &backgroundMusicHolder);

}

#endif