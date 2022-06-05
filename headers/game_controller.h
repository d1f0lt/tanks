#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <SFML/Graphics.hpp>
#include "model/client_game_model.h"

namespace Tanks {

struct GameController final {
public:
    static bool isEscReleased(const sf::Event &event);

    static void makeMove(model::PlayerActionsHandler &player);

    static void makeMove(model::PlayerActionsHandler &player,
                         model::Direction direction);

    static void makeShot(model::PlayerActionsHandler &player);
};

}  // namespace Tanks

#endif