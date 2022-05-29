#ifndef TANKS_SPAWNERS_H
#define TANKS_SPAWNERS_H

#include <memory>
#include "model/entity.h"
#include "model/game_model_fwd.h"

namespace Tanks::model {
class Spawner {
public:
    explicit Spawner(GameModel &model, int entityId);
    virtual ~Spawner() = default;

    void action();

    [[nodiscard]] virtual int getTimeout() = 0;
    [[nodiscard]] int getEntityId() const;
    [[nodiscard]] int getWaitingTime() const;

protected:
    [[nodiscard]] virtual std::unique_ptr<Entity> createEntity(int left,
                                                               int top) = 0;
    [[nodiscard]] GameModel &getModel();

private:
    GameModel &model_;
    const int entityId_;
    int waitingTime_ = 1;

    [[nodiscard]] std::pair<int, int> getFreeCoords();
};

class MediumTankSpawner : public Spawner {
protected:
    std::unique_ptr<Entity> createEntity(int left, int top) override;
};

}  // namespace Tanks::model

#endif  // TANKS_SPAWNERS_H
