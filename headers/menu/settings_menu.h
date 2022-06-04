#ifndef SETTINGS_MENU_H
#define SETTINGS_MENU_H

#include "menu.h"
#include "sound/background_music.h"

namespace Tanks::Menu {

void showSettingsMenu(sf::RenderWindow &window,
                      const sf::Sprite &backgroundSprite,
                      PlayerInfo &info,
                      Sound::BackgroundMusicHolder &backgroundMusicHolder);

}

#endif