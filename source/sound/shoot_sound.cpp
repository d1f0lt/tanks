#include "sound/shoot_sound.h"
#include <cassert>
#include "constants.h"

namespace Tanks::Sound {

ShootSoundHolder::ShootSoundHolder(const std::string &filename) {
    buffer.loadFromFile(filename);
    sound.setBuffer(buffer);
}

void ShootSoundHolder::play(float volume_level,
                            const std::vector<const model::Entity *> &bullets) {
    for (const auto *item : bullets) {
        sound.setVolume(volume_level);
        sound.play();
    }
}
};  // namespace Tanks::Sound