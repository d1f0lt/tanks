#include "model/spawners.h"
#include "model/game_model.h"
#include "model/tank.h"

namespace Tanks::model {
Spawner::Spawner(GameModel &model, int entityId)
    : model_(model), entityId_(entityId) {
}

GameModel &Spawner::getModel() {
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
    getModel().addEntity(createEntity(left, top));
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
    return std::make_unique<MediumTank>(left, top, getEntityId(),
                                        handlerCreator, Direction::LEFT,
                                        DEFAULT_TANK_SPEED);
}
}  // namespace Tanks::model
