#ifndef TANKS_PLAYER_SKILLS_H
#define TANKS_PLAYER_SKILLS_H

#include "constants.h"

namespace Tanks::Menu {
struct PlayerSkills {
    int tankSpeed = DEFAULT_TANK_SPEED;
    int bulletSpeed = DEFAULT_BULLET_SPEED;
    int reloadTicks = DEFAULT_RELOAD_TICKS;
};
}  // namespace Tanks::Menu

#endif  // TANKS_PLAYER_SKILLS_H
