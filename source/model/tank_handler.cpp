#include "model/tank_handler.h"
#include "model/bonus.h"
#include "model/game_model.h"
#include "model/projectile.h"
#include "model/tank.h"

namespace Tanks::model {
TankHandler::TankHandler(GameModel &model, Tank &entity)
    : MovableHandler(model, entity) {
}

void TankHandler::shoot() {
    auto &tank = dynamic_cast<Tank &>(getEntity());
    if (getModel().getTick() <= lastShootTick_ + tank.getReloadTicks()) {
        return;
    }

    getModel().wasShootThisTurn_ = true;
    lastShootTick_ = getModel().getTick();

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
    restoreBackground();
    if (!MovableHandler::moveOnly(direction, speed)) {
        return;
    }

    auto &model = getModel();

    setBackground();
    auto background = snapshotBackground();
    restoreBackground();
    auto &tank = dynamic_cast<Tank &>(getEntity());
    for (const auto &row : background) {
        for (int entityId : row) {
            auto entity = model.getById(entityId);
            if (!entity) {
                continue;
            }
            if (Bonus *bonus = dynamic_cast<Bonus *>(&entity->get())) {
                bonus->apply(tank);
            }
        }
    }
    setBackground();
}

void TankHandler::move(Direction direction) {
    move(direction, dynamic_cast<Tank &>(getEntity()).getSpeed());
}

void TankHandler::shoot(Direction direction) {
    setDirection(direction);
    shoot();
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

bool TankMovableOnWaterHandler::canStandOn(const Entity &other) const {
    return (other.isTankPassable()) || (other.getType() == EntityType::WATER);
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
