#include "model/spawners.h"
#include "model/bonus.h"
#include "model/event.h"
#include "model/server_game_model.h"
#include "model/tank.h"

namespace Tanks::model {
Spawner::Spawner(ServerModel &model, int entityId)
    : model_(model), entityId_(entityId) {
}

ServerModel &Spawner::getModel() {
    return model_;
}

int Spawner::getEntityId() const {
    return entityId_;
}

int Spawner::getWaitingTime() const {
    return waitingTime_;
}

std::pair<int, int> Spawner::getFreeCoords() {
    int left = -1;
    int top = -1;
    int right = -1;
    int down = -1;
    bool ok = false;

    auto entity = createEntity(0, 0);

    while (!ok) {
        left = getModel().getRnd() % getModel().getWidth();
        top = getModel().getRnd() % getModel().getHeight();
        right = std::min(left + entity->getWidth(), getModel().getWidth());
        down = std::min(top + entity->getHeight(), getModel().getHeight());
        ok = true;

        for (int row = top; row < down && ok; row++) {
            for (int col = left; col < right && ok; col++) {
                if (!entity->canStandOn(model_.getByCoords(col, row))) {
                    ok = false;
                }
            }
        }
    }
    return {left, top};
}

bool Spawner::isSpawnNow() {
    if (getModel().getById(getEntityId())) {
        return false;
    }
    return waitingTime_ == 0;
}

void Spawner::nextTick() {
    if (getModel().getById(getEntityId())) {
        waitingTime_ = getTimeout();
        return;
    }
    --waitingTime_;
}

std::unique_ptr<Entity> MediumTankSpawner::createEntity(int left, int top) {
    TankHandlerCreator handlerCreator(getModel());
    // TODO get PlayerInfo
    return std::make_unique<MediumTank>(left, top, DecrId(getEntityId()),
                                        handlerCreator, Direction::LEFT,
                                        DEFAULT_TANK_SPEED);
}

int MediumTankSpawner::getTimeout() {
    return DEFAULT_RESPAWN_TIME;
}

std::unique_ptr<Event> MediumTankSpawner::createEvent() {
    auto [left, top] = getFreeCoords();
    return std::make_unique<SpawnTank>(
        getEntityId(), left, top, EntityType::MEDIUM_TANK, DEFAULT_TANK_SPEED,
        DEFAULT_BULLET_SPEED, DEFAULT_RELOAD_TICKS);
}
MediumTankSpawner::MediumTankSpawner(ServerModel &model, int entityId)
    : Spawner(model, entityId) {
}

BonusSpawner::BonusSpawner(ServerModel &model, DecrId entityId, EntityType type)
    : Spawner(model, entityId), type_(type) {
}
int BonusSpawner::getTimeout() {
    return 0;
}
std::unique_ptr<Event> BonusSpawner::createEvent() {
    auto [left, top] = getFreeCoords();
    return std::make_unique<BonusSpawn>(getEntityId(), left, top, type_);
}

std::unique_ptr<Entity> BonusSpawner::createEntity(int left, int top) {
    switch (type_) {
        case (EntityType::WALK_ON_WATER_BONUS):
            return std::make_unique<WalkOnWater>(
                left, top, DecrId(getEntityId()), getModel());
        default:
            assert(false);
    }
}
}  // namespace Tanks::model
