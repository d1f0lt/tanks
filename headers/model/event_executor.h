#ifndef TANKS_EVENT_EXECUTOR_H
#define TANKS_EVENT_EXECUTOR_H

#include "model/event_fwd.h"
#include "model/game_model_fwd.h"

namespace Tanks::model {
class EventVisitor {
public:
    explicit EventVisitor(GameModel &model);

    [[nodiscard]] virtual bool visit(TankMove &event) const = 0;
    [[nodiscard]] virtual bool visit(TankSpawn &event) const = 0;
    [[nodiscard]] virtual bool visit(BonusSpawn &event) const = 0;
    [[nodiscard]] virtual bool visit(TankShoot &event) const = 0;
    [[nodiscard]] virtual bool visit(SetPosition &event) const = 0;
    [[nodiscard]] virtual bool visit(GameEnd &event) const = 0;

protected:
    [[nodiscard]] GameModel &getModel() const;

private:
    GameModel &model_;
};

class EventExecutor : public EventVisitor {
    friend GameModel;

protected:
    explicit EventExecutor(GameModel &model);

public:
    [[nodiscard]] bool visit(TankMove &event) const override;
    [[nodiscard]] bool visit(TankSpawn &event) const override;
    [[nodiscard]] bool visit(BonusSpawn &event) const override;
    [[nodiscard]] bool visit(TankShoot &event) const override;
    [[nodiscard]] bool visit(SetPosition &event) const override;
    [[nodiscard]] bool visit(GameEnd &event) const override;
};

class ServerEventExecutor : public EventExecutor {
    friend ServerModel;

public:
    explicit ServerEventExecutor(ServerModel &model);
    bool visit(TankSpawn &event) const override;
};
}  // namespace Tanks::model

#endif  // TANKS_EVENT_EXECUTOR_H
