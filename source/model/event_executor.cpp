#include "model/event_executor.h"
#include "model/game_model.h"

namespace Tanks::model {

EventExecutor::EventExecutor(GameModel &model) : model_(model) {
}

GameModel &EventExecutor::getModel() const {
    return model_;
}

void EventExecutor::execute(TankMove &event) const {
    auto &model = getModel();
    auto tank = model.getById(event.getId());
    if (!tank) {
        return;
    }

    dynamic_cast<TankHandler &>(model.getHandler(*tank))
        .move(event.getDirection(), event.getSpeed());
}
}  // namespace Tanks::model