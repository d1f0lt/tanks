#ifndef SHOOT_SOUND_H
#define SHOOT_SOUND_H

#include <SFML/Audio.hpp>
#include <vector>
#include "model/entity.h"

namespace Tanks::Sound {

struct ShootSoundHolder final {
    ShootSoundHolder(const std::string &filename);

    void play(float volume_level,
              const std::vector<const model::Entity *> &bullets);

private:
    sf::SoundBuffer buffer;
    sf::Sound sound;
};
}  // namespace Tanks::Sound

#endif  // SHOOT_SOUND_H
