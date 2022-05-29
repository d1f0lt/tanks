#include "model/event_executor.h"
#include "model/game_model.h"
#include "model/tank.h"

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

void EventExecutor::execute(SpawnTank &event) const {
    int id = event.getTankId();
    switch (event.getType()) {
        case (EntityType::MEDIUM_TANK):
            getModel().addEntity(std::make_unique<MediumTank>(
                event.getLeft(), event.getTop(), event.getTankId(),
                TankHandlerCreator(getModel()), Direction::LEFT, event.getTankSpeed()));
            break;
    }
}
}  // namespace Tanks::model
