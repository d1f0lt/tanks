#include "sound/background_music.h"
#include <cassert>
#include "constants.h"

namespace Tanks::Sound {

BackgroundMusicHolder::BackgroundMusicHolder(const std::string &filename) {
    music.openFromFile(filename);
}

void BackgroundMusicHolder::play(float volume_level) {
    music.setLoop(true);
    music.setVolume(volume_level);
    music.play();
}


void BackgroundMusicHolder::stop() {
    music.stop();
}

void BackgroundMusicHolder::setVolume(float volume_level) {
    music.setVolume(volume_level);
}
}  // namespace Tanks::Sound

