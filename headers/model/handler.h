#ifndef TANKS_HANDLER_H
#define TANKS_HANDLER_H

#include <climits>
#include <memory>
#include <unordered_set>
#include <vector>
#include "constants.h"
#include "entities_fwd.h"
#include "entity.h"
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

    virtual void destroyEntity();

protected:
    [[nodiscard]] Entity &getEntity() const;
    [[nodiscard]] GameModel &getModel() const;

    [[nodiscard]] static BasicHandler *getActualHandler(Entity &entity);

private:
    GameModel &model_;
    Entity &entity_;
};

class ForegroundHandler : public BasicHandler {
public:
    explicit ForegroundHandler(GameModel &model, ForegroundEntity &entity);

    void destroyEntity() override;

    void setBackground();
    void restoreBackground();
    [[nodiscard]] std::vector<int> snapshotBackground() const;

    void setPosition(int left, int top);

protected:
    [[nodiscard]] std::vector<int> &getBackground();
    [[nodiscard]] std::vector<int> underTank();

private:
    [[nodiscard]] const std::vector<int> &getBackground() const;

    [[nodiscard]] bool isDieOnCreation();
};

class MovableHandler : public ForegroundHandler {
public:
    explicit MovableHandler(GameModel &model, MovableEntity &entity);

    [[nodiscard]] std::vector<const Entity *> look(Direction direction);
    [[nodiscard]] virtual bool move(Direction direction, int speed);
    void setDirection(Direction direction);

protected:
    [[nodiscard]] bool moveOnly(Direction direction, int speed);

    [[nodiscard]] std::vector<Entity *> lookMutable(Direction direction);

    template <typename T>
    std::vector<Entity *> nearest(Direction direction, T cond) {
        int minDist = INT_MAX;
        std::vector<Entity *> res;
        auto forward = lookMutable(direction);
        for (auto *entity : forward) {
            if (cond(entity)) {
                if (getEntity().dist(*entity) <= minDist) {
                    if (int dist = getEntity().dist(*entity); dist < minDist) {
                        minDist = getEntity().dist(*entity);
                        res = {entity};
                    } else {
                        res.emplace_back(entity);
                    }
                }  // elements in forward are unsorted, can't else {break;}
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
    [[nodiscard]] bool canStandOn(const Entity &other) const override;
};

class WalkOnWaterHandler : public BonusHandler {
public:
    explicit WalkOnWaterHandler(GameModel &model, WalkOnWater &entity);

    void apply(Tank &tank) override;
};

}  // namespace Tanks::model
#endif  // TANKS_HANDLER_H
