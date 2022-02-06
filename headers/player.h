#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include "constants.h"
#include "tank.h"

namespace Tanks {

struct Player final : public Tank {
public:
    explicit Player(sf::Vector2<int> position);

private:
};

}  // namespace Tanks

#endif  // PLAYER_H_