#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <SFML/Graphics.hpp>
#include "model/playable-tank.h"

namespace Tanks {

struct GameController final {
public:
    static bool isEscReleased(const sf::Event &event);

    static void makeMove(model::PlayableTank &player);

    static void makeMove(model::PlayableTank &player,
                         model::Direction direction);
};

}  // namespace Tanks

#endif