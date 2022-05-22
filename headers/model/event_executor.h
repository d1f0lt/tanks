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
    void execute(TankMove &event) const;

protected:
    [[nodiscard]] GameModel &getModel() const;

private:
    GameModel &model_;
};
}  // namespace Tanks::model

#endif  // TANKS_EVENT_EXECUTOR_H
