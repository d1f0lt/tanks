#ifndef TANKS_PLAYER_SKILLS_H
#define TANKS_PLAYER_SKILLS_H

#include "constants.h"

namespace Tanks::Menu {
struct PlayerSkills {
    size_t tankSpeed = DEFAULT_TANK_SPEED;
    size_t bulletSpeed = DEFAULT_BULLET_SPEED;
    size_t reloadTicks = DEFAULT_RELOAD_TICKS;
    size_t lifeAmount = DEFAULT_LIVES_AMOUNT;
};
}  // namespace Tanks::Menu

#endif  // TANKS_PLAYER_SKILLS_H
