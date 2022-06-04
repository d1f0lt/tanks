#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "menu/menu.h"
#include "player_skills.h"

namespace Tanks {

std::optional<Menu::ButtonType> startGame(
    sf::RenderWindow &window,
    int level = 1,
    Menu::PlayerSkills skills = Menu::PlayerSkills(),
    std::optional<std::pair<std::string, std::string>> address = std::nullopt,
    int lives = INFINITE_LIVES);

}

#endif  // GAME_H_
