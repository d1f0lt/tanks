#ifndef PLAYER_H_
#define PLAYER_H_

#include "constants.h"
#include "tank.h"

namespace Tanks {

struct Player final : public Tank {
public:
    explicit Player(const std::string &filename, sf::Vector2<float> position);

    void change_tail() final;

private:
};

}  // namespace Tanks

#endif  // PLAYER_H_