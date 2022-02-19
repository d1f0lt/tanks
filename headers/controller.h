#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "player.h"
//#include "pause.h"

namespace Tanks {

struct GameController final {
public:
    static void checkPause();

    static void makeMove(Player &player, double time);
};

} // namespace Tanks

#endif // CONTROLLER_H