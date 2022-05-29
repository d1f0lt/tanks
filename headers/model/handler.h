#ifndef TANKS_HANDLER_H
#define TANKS_HANDLER_H

#include <climits>
#include <memory>
#include <vector>
#include "constants.h"
#include "model/entities_fwd.h"
#include "model/entity.h"
#include "model/game_model_fwd.h"

namespace Tanks::model {
class BasicHandler {
public:
    explicit BasicHandler(GameModel &model_, Entity &entity);

    BasicHandler(const BasicHandler &) = delete;
    BasicHandler(BasicHandler &&) = delete;
    BasicHandler &operator=(const BasicHandler &) = delete;
    BasicHandler &operator=(BasicHandler &&) = delete;

    virtual ~BasicHandler();

    [[nodiscard]] virtual bool canStandOn(const Entity &other) const;

    virtual void destroyByBullet();
    virtual void destroyEntity();

protected:
    [[nodiscard]] Entity &getEntity() const;
    [[nodiscard]] GameModel &getModel() const;

private:
    GameModel &model_;
    Entity &entity_;
};

class ForegroundHandler : public BasicHandler {
public:
    explicit ForegroundHandler(GameModel &model, ForegroundEntity &entity);

    void setBackground();
    void restoreBackground();
    void destroyEntity() override;
    [[nodiscard]] std::vector<std::vector<const Entity *>> snapshotBackground()
        const;

    void setPosition(int left, int top);

    void destroyByBullet() override;

private:
    std::vector<std::vector<Entity *>> background_;
};

class MovableHandler : public ForegroundHandler {
public:
    explicit MovableHandler(GameModel &model, MovableEntity &entity);

    [[nodiscard]] std::vector<const Entity *> look(Direction direction);
    virtual void move(Direction direction, int speed);
    void setDirection(Direction direction);

protected:
    [[nodiscard]] std::vector<Entity *> lookMutable(Direction direction);

    template <typename T>
    std::vector<Entity *> nearest(Direction direction, T cond) {
        int minDist = INT_MAX;
        std::vector<Entity *> res;
        for (auto *entity : lookMutable(direction)) {
            if (cond(entity)) {
                if (getEntity().dist(*entity) <= minDist) {
                    if (int dist = getEntity().dist(*entity); dist < minDist) {
                        minDist = getEntity().dist(*entity);
                        res = {entity};
                    } else {
                        res.emplace_back(entity);
                    }
                } else {
                    break;  // TODO check it
                }
            }
        }
        return res;
    }
};

class ProjectileHandler : public MovableHandler {
public:
    explicit ProjectileHandler(GameModel &model, MovableEntity &entity);

    void interactOnNextTick();

    [[nodiscard]] bool isBreakOnCreation();

    [[nodiscard]] bool canStandOn(const Entity &other) const override;

protected:
    [[nodiscard]] bool breakIfBreakable();
    void destroy(Entity &other);
};

class BonusHandler : public ForegroundHandler {
public:
    explicit BonusHandler(GameModel &model, Bonus &entity);

    virtual void apply(Tank &tank) = 0;
};

class WalkOnWaterHandler : public BonusHandler {
public:
    explicit WalkOnWaterHandler(GameModel &model, WalkOnWater &entity);

    void apply(Tank &tank) override;
};

}  // namespace Tanks::model
#endif  // TANKS_HANDLER_H
