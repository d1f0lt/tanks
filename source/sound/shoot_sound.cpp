#include "sound/shoot_sound.h"
#include <cassert>
#include "constants.h"
#include "model/game_model.h"
#include "model/handler.h"
#include "model/player_action_handler.h"

namespace Tanks::Sound {

using model::PlayerActionsHandler;

ShootSoundHolder::ShootSoundHolder(const std::string &filename) {
    buffer.loadFromFile(filename);
    sound.setBuffer(buffer);
}

void ShootSoundHolder::play(float volume_level, const PlayerActionsHandler &playerActionsHandler) {
    if (playerActionsHandler.getModel().wasShootThisTurn()){
        sound.setVolume(volume_level);
        sound.play();
    }
}
};