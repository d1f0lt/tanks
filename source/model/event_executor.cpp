#include "model/event_executor.h"
#include "model/bonus.h"
#include "model/game_model.h"
#include "model/server_game_model.h"
#include "model/tank.h"

namespace Tanks::model {

EventVisitor::EventVisitor(GameModel &model) : model_(model) {
}

GameModel &EventVisitor::getModel() const {
    return model_;
}

bool EventExecutor::visit(TankMove &event) const {
    auto &model = getModel();
    auto tank = model.getById(event.getId());
    if (tank == std::nullopt) {
        return false;
    }
    assert(getModel().byId_.count(event.getId()));
    assert(tank != std::nullopt);
    dynamic_cast<TankHandler &>(model.getHandler(tank->get()))
        .move(event.getDirection(), event.getSpeed());
    return true;
}

bool EventExecutor::visit(TankSpawn &event) const {
    if (getModel().getById(event.getTankId())) {
        return false;
    }

    switch (event.getEntityType()) {
        case (EntityType::MEDIUM_TANK):
            getModel().addEntity(std::make_unique<MediumTank>(
                event.getLeft(), event.getTop(), event.getTankId(),
                TankHandlerCreator(getModel()), Direction::LEFT,
                event.getTankSpeed(), event.getReloadTicks(),
                event.getBulletSpeed()));
            break;
        default:
            assert(false);
            return false;
    }
    return true;
}

bool EventExecutor::visit(BonusSpawn &event) const {
    auto entity = getModel().getById(event.getId());
    if (entity) {
        return false;
    }
    switch (event.getEntityType()) {
        case (EntityType::WALK_ON_WATER_BONUS):
            getModel().addEntity(std::make_unique<WalkOnWater>(
                event.getLeft(), event.getTop(), event.getId(), getModel()));
            break;
        default:
            assert(false);
            return false;
    }
    return true;
}

bool EventExecutor::visit(TankShoot &event) const {
    auto tank = getModel().getById(event.getTankId());
    if (!tank) {
        return false;
    }

    dynamic_cast<TankHandler &>(getModel().getHandler(tank->get()))
        .shoot(event.getDirection());
    return true;
}

bool EventExecutor::visit(SetPosition &event) const {
    auto entity = getModel().getById(event.getId());
    if (!entity) {
        return false;
    }
    auto &handler =
        dynamic_cast<ForegroundHandler &>(getModel().getHandler(entity->get()));
    handler.setPosition(event.getLeft(), event.getTop());
    return true;
}

bool EventExecutor::visit(GameEnd &) const {
    getModel().finishGame();
    return true;
}

EventExecutor::EventExecutor(GameModel &model) : EventVisitor(model) {
}

ServerEventExecutor::ServerEventExecutor(ServerModel &model)
    : EventExecutor(model) {
}

bool ServerEventExecutor::visit(TankSpawn &event) const {
    dynamic_cast<ServerModel &>(getModel()).decrLives(event.getTankId());
    return EventExecutor::visit(event);
}
}  // namespace Tanks::model
