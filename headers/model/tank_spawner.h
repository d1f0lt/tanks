/*
#ifndef TANKS_TANK_SPAWNER_H
#define TANKS_TANK_SPAWNER_H

#include "model/handler.h"
#include "model/spawners.h"

namespace Tanks::model {
template <typename TankType>
class TankSpawner : public Spawner {
public:
    explicit TankSpawner(GameModel &model, int entityId)
        : Spawner(model, entityId) {
    }

protected:
    [[nodiscard]] std::unique_ptr<Entity> createEntity(int left,
                                                       int top) final {
        return std::make_unique<TankType>(
            dynamic_cast<TankType &>(left, top, getEntityId(), Direction::LEFT,
                                     DEFAULT_TANK_SPEED, creator_));
    }

    [[nodiscard]] int getTimeout() override {
        return 10;  // TODO constant
    }

private:
    TankHandlerCreator creator_;
};
}  // namespace Tanks::model

#endif  // TANKS_TANK_SPAWNER_H
*/
