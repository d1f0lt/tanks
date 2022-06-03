#include "sound/block_destroy_sound.h"
#include <cassert>
#include "constants.h"

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
