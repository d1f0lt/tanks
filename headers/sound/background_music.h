#ifndef BACKGROUND_MUSIC_H
#define BACKGROUND_MUSIC_H

#include <SFML/Audio.hpp>
#include <vector>
#include "model/entity.h"


namespace Tanks::Sound {

struct BackgroundMusicHolder final {
    BackgroundMusicHolder(const std::string &filename);

    void play(float volume_level);

private:
    sf::Music music;
};
};

#endif  // BACKGROUND_MUSIC_H

