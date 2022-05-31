#ifndef MOVING_TANK_SOUND_H
#define MOVING_TANK_SOUND_H

#include <SFML/Audio.hpp>
#include <vector>
#include "model/entity.h"


namespace Tanks::Sound {

struct ShootSoundHolder final {
    ShootSoundHolder(const std::string &filename);

    void play(float volume_level);

private:
    sf::SoundBuffer buffer;
    sf::Sound sound();
};
};

#endif  // MOVING_TANK_SOUND_H
