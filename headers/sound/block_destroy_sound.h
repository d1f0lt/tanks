#ifndef BLOCK_DESTROY_SOUND_H
#define BLOCK_DESTROY_SOUND_H

#include <SFML/Audio.hpp>
#include <vector>
#include "model/entity.h"

namespace Tanks::Sound {

struct BlockDestroySoundHolder final {
    BlockDestroySoundHolder(const std::string &filename);

    void play(float volume_level);

private:
    sf::SoundBuffer buffer;
    sf::Sound sound;
};
}

#endif  // BLOCK_DESTROY_SOUND_H
