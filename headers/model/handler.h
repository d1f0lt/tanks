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

    [[nodiscard]] std::vector<std::vector<const Entity *>> snapshotBackground()
        const;

private:
    std::vector<std::vector<Entity *>> background_;
};

class MovableHandler : public ForegroundHandler {
public:
    explicit MovableHandler(GameModel &model, MovableEntity &entity);

    [[nodiscard]] std::vector<const Entity *> look(Direction direction);
    virtual void move(Direction direction, int speed);

protected:
    [[nodiscard]] std::vector<Entity *> lookMutable(Direction direction);

    template <typename T>
    std::vector<Entity *> nearest(Direction direction, T cond) {
        int dist = INT_MAX;
        std::vector<Entity *> res;
        for (auto *entity : lookMutable(direction)) {
            if (cond(entity)) {
                if (getEntity().dist(*entity) <= dist) {
                    if (getEntity().dist(*entity) < dist) {
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

class TankHandler : public MovableHandler {
public:
    explicit TankHandler(GameModel &model, Tank &entity);

    void move(Direction dir, int speed) final;
    void move(Direction direction);
    void shoot();

private:
    int lastMoveTick_ = -1;
    int lastShootTick_ = -RELOAD_TICKS - 1;
};

class ProjectileHandler : public MovableHandler {
public:
    explicit ProjectileHandler(GameModel &model, MovableEntity &entity);

    void interactOnNextTick();

    [[nodiscard]] bool breakIfBreakable();
    [[nodiscard]] bool isBreakOnCreation();

protected:
    void destroyByBullet(Entity &other);
};

}  // namespace Tanks::model
#endif  // TANKS_HANDLER_H
