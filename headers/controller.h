#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "player.h"

namespace Tanks {

struct GameController final {
public:
    static bool isEscReleased(const sf::Event &event);

    static void makeMove(Player &player, double time);
};

} // namespace Tanks

#endif // CONTROLLER_H