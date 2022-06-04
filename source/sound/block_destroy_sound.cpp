#include "sound/block_destroy_sound.h"
#include <cassert>
#include "constants.h"
#include "model/game_model.h"
#include "model/player_action_handler.h"

namespace Tanks::Sound {

BlockDestroySoundHolder::BlockDestroySoundHolder(const std::string &filename) {
    buffer.loadFromFile(filename);
    sound.setBuffer(buffer);
}

void BlockDestroySoundHolder::play(float volume_level, const PlayerActionsHandler &playerActionsHandler){
    if (playerActionsHandler.getModel().wasDestroyedBlockThisTurn()) {
        sound.setVolume(volume_level);
        sound.play();
    }
}
};
