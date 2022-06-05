#ifndef SHOOT_SOUND_H
#define SHOOT_SOUND_H

#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "model/entity.h"
#include "model/player_action_handler.h"

namespace Tanks::Sound {

using model::PlayerActionsHandler;

struct ShootSoundHolder final {
    ShootSoundHolder(const std::string &filename);

    void play(float volume_level, const PlayerActionsHandler &playerActionsHandler);

private:
    sf::SoundBuffer buffer;
    sf::Sound sound;
};
}  // namespace Tanks::Sound

#endif  // SHOOT_SOUND_H
