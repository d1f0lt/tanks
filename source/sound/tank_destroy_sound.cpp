#include "sound/tank_destroy_sound.h"
#include "model/game_model.h"
#include "model/handler.h"
#include "model/player_action_handler.h"

namespace Tanks::Sound {

using model::PlayerActionsHandler;

TankDestroySoundHolder::TankDestroySoundHolder(const std::string &filename) {
    buffer.loadFromFile(filename);
    sound.setBuffer(buffer);
}

void TankDestroySoundHolder::play(float volume_level, const PlayerActionsHandler &playerActionsHandler) {
    if (playerActionsHandler.getModel().wasTankDestroyedThisTurn()) {
        sound.setVolume(volume_level);
        sound.play();
    }
}
}  // namespace Tanks::Sound
