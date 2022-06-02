#include "sound/tank_destroy_sound.h"
#include <cassert>
#include "constants.h"

namespace Tanks::Sound {

TankDestroySoundHolder::TankDestroySoundHolderr(const std::string &filename) {
    buffer.loadFromFile(filename);
    sound.setBuffer(buffer);
}

void TankDestroySoundHolder::play(float volume_level) {
    if () {
        sound.setVolume(volume_level);
        sound.play();
    }
}
};
