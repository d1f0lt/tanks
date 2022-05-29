#include "model/spawners.h"
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

void Spawner::action() {
    if (getModel().getById(getEntityId())) {
        return;
    }

    if (waitingTime_ == 0) {
        waitingTime_ = getTimeout() + 1;
    }

    if (--waitingTime_ > 0) {
        return;
    }

    assert(waitingTime_ == 0);
    auto [left, top] = getFreeCoords();
    //    getModel().events_.push(std::make_unique<)
    getModel().events_.emplace(createEvent(left, top));
    //    getModel().addEntity(createEntity(left, top));
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
                if (!entity->canPass(model_.getByCoords(col, row))) {
                    ok = false;
                }
            }
        }
    }
    return {left, top};
}

std::unique_ptr<Entity> MediumTankSpawner::createEntity(int left, int top) {
    TankHandlerCreator handlerCreator(getModel());
    // TODO get PlayerInfo
    return std::make_unique<MediumTank>(left, top, getEntityId(),
                                        handlerCreator, Direction::LEFT,
                                        DEFAULT_TANK_SPEED);
}
int MediumTankSpawner::getTimeout() {
    return 10;
}

std::unique_ptr<Event> MediumTankSpawner::createEvent(int left, int top) {
    return std::make_unique<SpawnTank>(getEntityId(), left, top,
                                       EntityType::MEDIUM_TANK);
}
MediumTankSpawner::MediumTankSpawner(ServerModel &model, int entityId)
    : Spawner(model, entityId) {
}
}  // namespace Tanks::model
