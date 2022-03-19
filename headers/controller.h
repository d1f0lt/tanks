#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "pause.h"
#include "player.h"

namespace Tanks {

struct GameController final {
public:
    static bool isEscReleased(const sf::Event &event);

    static void makeMove(Player &player, double time);
};

struct PauseController final {

    [[nodiscard]] static std::optional<Button> control(
        Pause &pause,
        sf::RenderWindow &window);
};

}  // namespace Tanks

#endif  // CONTROLLER_H