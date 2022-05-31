#ifndef TANKS_EVENT_EXECUTOR_H
#define TANKS_EVENT_EXECUTOR_H

#include "model/event_fwd.h"
#include "model/game_model_fwd.h"

namespace Tanks::model {
class EventExecutor {
    friend GameModel;

private:
    explicit EventExecutor(GameModel &model);

public:
    [[nodiscard]] bool execute(TankMove &event) const;
    [[nodiscard]] bool execute(SpawnTank &event) const;
    [[nodiscard]] bool execute(BonusSpawn &event) const;
    [[nodiscard]] bool execute(TankShoot &event) const;
    [[nodiscard]] bool execute(SetPosition &event) const;

protected:
    [[nodiscard]] GameModel &getModel() const;

private:
    GameModel &model_;
};
}  // namespace Tanks::model

#endif  // TANKS_EVENT_EXECUTOR_H
