#include "model/tank_handler.h"
#include "model/game_model.h"
#include "model/projectile.h"
#include "model/tank.h"

namespace Tanks::model {
TankHandler::TankHandler(GameModel &model, Tank &entity)
    : MovableHandler(model, entity) {
}

void TankHandler::shoot() {
    if (getModel().getTick() <= lastShootTick_ + RELOAD_TICKS) {
        return;
    }

    lastShootTick_ = getModel().getTick();

    auto &tank = dynamic_cast<Tank &>(getEntity());

    static const std::unordered_map<Direction, int> DCOL = {
        {Direction::UP, getEntity().getWidth() / 2},
        {Direction::DOWN, getEntity().getWidth() / 2},
        {Direction::RIGHT, getEntity().getWidth()},
        {Direction::LEFT, -1}};

    static const std::unordered_map<Direction, int> DROW = {
        {Direction::UP, -1},
        {Direction::DOWN, getEntity().getHeight()},
        {Direction::RIGHT, getEntity().getHeight() / 2},
        {Direction::LEFT, getEntity().getHeight() / 2}};

    auto projectile = std::make_unique<Projectile>(
        getEntity().getLeft() + DCOL.at(tank.getDirection()),
        getEntity().getTop() + DROW.at(tank.getDirection()),
        tank.getDirection(), getModel(), getModel().getIncrId());

    auto &handler =
        dynamic_cast<ProjectileHandler &>(getModel().getHandler(*projectile));

    if (handler.isBreakOnCreation()) {
        return;
    }

    getModel().addEntity(std::move(projectile));
}

void TankHandler::move(Direction direction, int speed) {
    if (getModel().getTick() <= std::max(lastShootTick_, lastMoveTick_)) {
        return;
    }

    lastMoveTick_ = getModel().getTick();
    MovableHandler::move(direction, speed);
}

void TankHandler::move(Direction direction) {
    move(direction, dynamic_cast<Tank &>(getEntity()).getSpeed());
}

void TankMovableOnWaterHandler::stopBonus() {
}

TankMovableOnWaterHandler::TankMovableOnWaterHandler(GameModel &model,
                                                     Tank &entity,
                                                     int beginLive)
    : TankHandler(model, entity), beginLive_(beginLive) {
}

void TankMovableOnWaterHandler::move(Direction dir, int speed) {
    TankHandler::move(dir, speed);
    stopBonus();
}

void TankMovableOnWaterHandler::shoot() {
    TankHandler::shoot();
    stopBonus();
}

std::unique_ptr<TankHandler> TankHandlerCreator::createTankHandler(
    Tank &tank) const {
    return std::make_unique<TankHandler>(model_, tank);
}

GameModel &TankHandlerCreator::getModel() const {
    return model_;
}

TankHandlerCreator::TankHandlerCreator(GameModel &model) : model_(model) {
}

TankMovableOnWaterHandlerCreator::TankMovableOnWaterHandlerCreator(
    GameModel &model)
    : TankHandlerCreator(model) {
}

std::unique_ptr<TankHandler>
TankMovableOnWaterHandlerCreator::createTankHandler(Tank &tank) const {
    return std::make_unique<TankMovableOnWaterHandler>(getModel(), tank,
                                                       getModel().getTick());
}
}  // namespace Tanks::model
