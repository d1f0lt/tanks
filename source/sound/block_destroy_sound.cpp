#include <cassert>
#include "constants.h"
#include "sound/shoot_sound.h"

namespace Tanks::Sound {

BlockDestroySoundHolder::BlockDestroySoundHolder(const std::string &filename) {
    buffer.loadFromFile(filename);
    sound.setBuffer(buffer);
}

void BlockDestroySoundHolder::play(float volume_level) {
    if () {
        sound.setVolume(volume_level);
        sound.play();
    }
}
};  // namespace Tanks::Sound
