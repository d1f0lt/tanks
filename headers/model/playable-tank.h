#ifndef TANKS_PLAYABLE_TANK_H
#define TANKS_PLAYABLE_TANK_H

#include <memory>
#include <queue>
#include "model/playable_tank_handler.h"
#include "model/tank.h"

namespace Tanks::model {
class PlayableTank : public Tank {
public:
    explicit PlayableTank(int left,
                          int top,
                          int entityId,
                          std::unique_ptr<PlayableTankHandler> handler,
                          Direction direction);

    explicit PlayableTank(int left,
                          int top,
                          int entityId,
                          Direction direction,
                          boost::asio::ip::tcp::socket &is,
                          GameModel &model);

    [[nodiscard]] EntityType getType() const override;

    void move(Direction direction, int speed) override;

    void move(Direction direction);
};
}  // namespace Tanks::model
#endif  // TANKS_PLAYABLE_TANK_H
