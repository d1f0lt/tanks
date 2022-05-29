#include "model/event_executor.h"
#include "model/bonus.h"
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
    if (getModel().getById(event.getTankId())) {
        return;
    }

    switch (event.getType()) {
        case (EntityType::MEDIUM_TANK):
            getModel().addEntity(std::make_unique<MediumTank>(
                event.getLeft(), event.getTop(), event.getTankId(),
                TankHandlerCreator(getModel()), Direction::LEFT,
                event.getTankSpeed()));
            break;
        default:
            assert(false);
    }
}

void EventExecutor::execute(BonusSpawn &event) const {
    auto entity = getModel().getById(event.getId());
    if (entity) {
        return;
    }
    switch (event.getType()) {
        case (EntityType::WALK_ON_WATER_BONUS):
            getModel().addEntity(std::make_unique<WalkOnWater>(
                event.getLeft(), event.getTop(), event.getId(), getModel()));
            break;
        default:
            assert(false);
            return;
    }
}

void EventExecutor::execute(TankShoot &event) const {
    auto tank = getModel().getById(event.getTankId());
    if (!tank) {
        return;
    }

    dynamic_cast<TankHandler &>(getModel().getHandler(tank->get()))
        .shoot(event.getDirection());
}

void EventExecutor::execute(SetPosition &event) const {
    auto entity = getModel().getById(event.getId());
    if (!entity) {
        return;
    }
    auto &handler =
        dynamic_cast<ForegroundHandler &>(getModel().getHandler(entity->get()));
    handler.setPosition(event.getLeft(), event.getTop());
}
}  // namespace Tanks::model
