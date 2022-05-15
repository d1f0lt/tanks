#ifndef TANKS_HANDLER_H
#define TANKS_HANDLER_H

#include <climits>
#include <ostream>
#include <vector>
#include "constants.h"
#include "entity.h"
#include "model/entities_fwd.h"

namespace Tanks::model {
class GameModel;  // TODO GameModel fwd
class BasicHandler {
public:
    explicit BasicHandler(GameModel &model_, Entity &entity);

    BasicHandler(const BasicHandler &) = delete;
    BasicHandler(BasicHandler &&) = delete;
    BasicHandler &operator=(const BasicHandler &) = delete;
    BasicHandler &operator=(BasicHandler &&) = delete;

    virtual ~BasicHandler() = default;

protected:
    GameModel &
        model_;  // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
    Entity &
        entity_;  // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
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
                if (entity_.dist(*entity) <= dist) {
                    if (entity_.dist(*entity) == dist) {
                        res = {entity};
                    } else {
                        res.push_back(entity);
                    }
                }
            }
        }
        return res;
    }
};

class TankHandler : public MovableHandler {
public:
    explicit TankHandler(GameModel &model, Tank &entity);

    void move(Direction dir, int speed) override;
    void move(Direction direction);
    void shoot();

private:
    int lastMoveTick = -1;
    int lastShootTick = -RELOAD_TICKS - 1;
};

class PlayableTankHandler : public TankHandler {
public:
    explicit PlayableTankHandler(GameModel &model,
                                 PlayableTank &tank,
                                 std::ostream &os);

    void sendMove(Direction direction, int speed);

private:
    std::ostream &os_;
};

class ProjectileHandler : public MovableHandler {
public:
    explicit ProjectileHandler(GameModel &model, MovableEntity &entity);

    [[nodiscard]] bool isBreakOnNextTick();
    [[nodiscard]] bool isBreakOnCreation();

protected:
    void destroyByBullet(Entity &other);
};

}  // namespace Tanks::model
#endif  // TANKS_HANDLER_H
