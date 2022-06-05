#ifndef TANKS_TANK_HANDLER_H
#define TANKS_TANK_HANDLER_H

#include "model/handler.h"

namespace Tanks::model {
class TankHandler : public MovableHandler {
public:
    explicit TankHandler(GameModel &model, Tank &entity);

    [[nodiscard]] bool move(Direction direction, int speed) override;
    [[nodiscard]] bool move(Direction direction);
    virtual void shoot();
    void shoot(Direction direction);

    void applyBonusesInBackground();
    [[nodiscard]] bool isShootingThisTick() const;

    void destroyEntity() override;

private:
    int lastMoveTick_ = -1;
    int lastShootTick_ = -DEFAULT_RELOAD_TICKS - 1;
};

class TankMovableOnWaterHandler : public TankHandler {
public:
    explicit TankMovableOnWaterHandler(GameModel &model,
                                       Tank &entity,
                                       int beginLive);

    bool move(Direction dir, int speed) override;
    void shoot() final;

    [[nodiscard]] bool canStandOn(const Entity &other) const override;

private:
    const int beginLive_;

    void stopBonus();
};

class TankHandlerCreator {
public:
    explicit TankHandlerCreator(GameModel &model);
    virtual ~TankHandlerCreator() = default;

    [[nodiscard]] virtual std::unique_ptr<TankHandler> createTankHandler(
        Tank &tank) const;

protected:
    [[nodiscard]] GameModel &getModel() const;

private:
    GameModel &model_;
};

class TankMovableOnWaterHandlerCreator : public TankHandlerCreator {
public:
    explicit TankMovableOnWaterHandlerCreator(GameModel &model);

private:
    [[nodiscard]] std::unique_ptr<TankHandler> createTankHandler(
        Tank &tank) const override;
};
}  // namespace Tanks::model

#endif  // TANKS_TANK_HANDLER_H
