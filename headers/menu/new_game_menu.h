#ifndef NEW_GAME_MENU_H
#define NEW_GAME_MENU_H

#include "menu/menu.h"
#include "menu.h"
#include "sound/background_music.h"

namespace Tanks::Menu {

void showNewGameMenu(sf::RenderWindow &window,
                     const sf::Sprite &background,
                     PlayerInfo &info,
                     Sound::BackgroundMusicHolder &backgroundMusicHolder);

}

#endif