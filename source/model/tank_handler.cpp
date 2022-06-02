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

    static const std::unordered_map<Direction, int> DIFF_LEFT = {
        {Direction::UP, getEntity().getWidth() / 2 - 5},
        {Direction::DOWN, getEntity().getWidth() / 2 - 5},
        {Direction::RIGHT, getEntity().getWidth() + 4},
        {Direction::LEFT, -1 - 11}};

    static const std::unordered_map<Direction, int> DIFF_TOP = {
        {Direction::UP, -1 - 9},
        {Direction::DOWN, getEntity().getHeight() + 3},
        {Direction::RIGHT, getEntity().getHeight() / 2 - 12},
        {Direction::LEFT, getEntity().getHeight() / 2 - 12}};

    auto projectile = std::make_unique<Projectile>(
        getEntity().getLeft() + DIFF_LEFT.at(tank.getDirection()),
        getEntity().getTop() + DIFF_TOP.at(tank.getDirection()),
        tank.getDirection(), getModel(), getModel().getIncrId());

    auto &handler =
        dynamic_cast<ProjectileHandler &>(getModel().getHandler(*projectile));

    if (handler.isBreakOnCreation()) {
        return;
    }

    getModel().addEntity(std::move(projectile));
}

bool TankHandler::move(Direction direction, int speed) {
    if (getModel().getTick() <= std::max(lastShootTick_, lastMoveTick_)) {
        return false;
    }

    lastMoveTick_ = getModel().getTick();
    restoreBackground();
    if (!MovableHandler::moveOnly(direction, speed)) {
        return false;
    }
    applyBonusesInBackground();
    return true;
}

bool TankHandler::move(Direction direction) {
    return move(direction, dynamic_cast<Tank &>(getEntity()).getSpeed());
}

void TankHandler::shoot(Direction direction) {
    setDirection(direction);
    shoot();
}

void TankHandler::applyBonusesInBackground() {
    assert(getBackground().empty());
    auto background = underTank();
    auto &model = getModel();
    auto &tank = dynamic_cast<Tank &>(getEntity());
    std::vector<Bonus *> bonuses;
    for (unsigned i = 0; i < background.size(); i++) {
        auto entity = model.getById(background[i]);
        if (!entity) {
            continue;
        }
        Bonus *bonus = nullptr;
        if (bonus = dynamic_cast<Bonus *>(&entity->get()); bonus == nullptr) {
            continue;
        }
        bonuses.emplace_back(bonus);
    }
    if (bonuses.empty()) {
        setBackground();
        return;
    }

    for (auto *bonus : bonuses) {
        bonus->apply(tank);
    }
    dynamic_cast<TankHandler *>(getActualHandler(tank))->setBackground();
}

bool TankHandler::isShootingThisTick() const {
    return getModel().getTick() == lastShootTick_;
}

void TankMovableOnWaterHandler::stopBonus() {
}

TankMovableOnWaterHandler::TankMovableOnWaterHandler(GameModel &model,
                                                     Tank &entity,
                                                     int beginLive)
    : TankHandler(model, entity), beginLive_(beginLive) {
}

bool TankMovableOnWaterHandler::move(Direction dir, int speed) {
    auto &entity = getEntity();
    if (!TankHandler::move(dir, speed)) {
        return false;
    }
    if (getActualHandler(entity) != this) {
        return true;
    }
    stopBonus();
    return true;
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
