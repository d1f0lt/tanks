#ifndef TANK_DESTROY_SOUND_H
#define TANK_DESTROY_SOUND_H

#include <SFML/Audio.hpp>
#include <vector>
#include "model/entity.h"

namespace Tanks::Sound {

struct TankDestroySoundHolder final {
    TankDestroySoundHolder(const std::string &filename);

    void play(float volume_level);

private:
    sf::SoundBuffer buffer;
    sf::Sound sound;
};
}

#endif  // TANK_DESTROY_SOUND_H
