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

    void action();

    [[nodiscard]] virtual int getTimeout() = 0;
    [[nodiscard]] int getEntityId() const;
    [[nodiscard]] int getWaitingTime() const;
    [[nodiscard]] ServerModel &getModel();

protected:
    [[nodiscard]] virtual std::unique_ptr<Entity> createEntity(int left,
                                                               int top) = 0;

    [[nodiscard]] virtual std::unique_ptr<Event> createEvent(int left,
                                                             int top) = 0;

private:
    ServerModel &model_;
    const int entityId_;
    int waitingTime_ = 1;

    [[nodiscard]] std::pair<int, int> getFreeCoords();
};

class MediumTankSpawner : public Spawner {
public:
    [[nodiscard]] int getTimeout() override;
    explicit MediumTankSpawner(ServerModel &model, int entityId);

protected:
    [[nodiscard]] std::unique_ptr<Entity> createEntity(int left,
                                                       int top) override;
    [[nodiscard]] std::unique_ptr<Event> createEvent(int left,
                                                     int top) override;
};

}  // namespace Tanks::model

#endif  // TANKS_SPAWNERS_H
