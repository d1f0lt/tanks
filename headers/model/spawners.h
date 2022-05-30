#ifndef TANKS_SPAWNERS_H
#define TANKS_SPAWNERS_H

#include <memory>
#include "model/entity.h"
#include "model/event_fwd.h"
#include "model/game_model_fwd.h"

namespace Tanks::model {
class Spawner {
public:
    explicit Spawner(ServerModel &model, int entityId);
    virtual ~Spawner() = default;

    [[nodiscard]] bool isSpawnNow();
    void nextTick();

    [[nodiscard]] virtual int getTimeout() = 0;
    [[nodiscard]] int getEntityId() const;
    [[nodiscard]] int getWaitingTime() const;
    [[nodiscard]] ServerModel &getModel();
    [[nodiscard]] virtual std::unique_ptr<Event> createEvent() = 0;

protected:
    [[nodiscard]] virtual std::unique_ptr<Entity> createEntity(int left,
                                                               int top) = 0;

    [[nodiscard]] std::pair<int, int> getFreeCoords();

private:
    ServerModel &model_;
    const int entityId_;
    int waitingTime_ = 0;
};

class MediumTankSpawner : public Spawner {
public:
    [[nodiscard]] int getTimeout() override;
    explicit MediumTankSpawner(ServerModel &model, int entityId);

protected:
    [[nodiscard]] std::unique_ptr<Entity> createEntity(int left,
                                                       int top) override;
    [[nodiscard]] std::unique_ptr<Event> createEvent() override;
};

class BonusSpawner : public Spawner {
public:
    explicit BonusSpawner(ServerModel &model, DecrId entityId, EntityType type);

    [[nodiscard]] int getTimeout() override;
    [[nodiscard]] std::unique_ptr<Event> createEvent() override;

protected:
    [[nodiscard]] std::unique_ptr<Entity> createEntity(int left,
                                                       int top) override;

private:
    const EntityType type_;
};

}  // namespace Tanks::model

#endif  // TANKS_SPAWNERS_H