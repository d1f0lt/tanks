#ifndef TANK_DESTROY_SOUND_H
#define TANK_DESTROY_SOUND_H

#include <SFML/Audio.hpp>
#include <vector>
#include "model/entity.h"
#include "model/player_action_handler.h"

namespace Tanks::Sound {

using model::PlayerActionsHandler;

struct TankDestroySoundHolder final {
    TankDestroySoundHolder(const std::string &filename);

    void play(float volume_level, const PlayerActionsHandler &playerActionsHandler);

private:
    sf::SoundBuffer buffer;
    sf::Sound sound;
};
}  // namespace Tanks::Sound

#endif  // TANK_DESTROY_SOUND_H
