#ifndef MULTIPLAYER_MENU_H
#define MULTIPLAYER_MENU_H

#include "menu/menu.h"
#include "menu.h"
#include "sound/background_music.h"

namespace Tanks::Menu {

void showMultiplayerMenu(sf::RenderWindow &window,
                     const sf::Sprite &background,
                     PlayerInfo &info,
                     Sound::BackgroundMusicHolder &backgroundMusicHolder);

}

#endif