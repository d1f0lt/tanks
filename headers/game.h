#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "menu/menu.h"
#include "player_skills.h"
#include "database.h"

namespace Tanks {

std::optional<Menu::ButtonType> startGame(
    sf::RenderWindow &window,
    Menu::PlayerInfo &info,
    Sound::BackgroundMusicHolder &backgroundMusicHolder,
    const sf::Sprite &backgroundSprite,
    int level = 1,
    Menu::PlayerSkills skills = Menu::PlayerSkills(),
    std::optional<std::pair<std::string, std::string>> address = std::nullopt,
    int players = 1);

}

#endif  // GAME_H_
