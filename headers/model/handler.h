#ifndef TANKS_HANDLER_H
#define TANKS_HANDLER_H

#include <climits>
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
    GameModel &model_;  // NOLINT(misc-non-private-member-variables-in-classes)
    Entity &entity_;    // NOLINT(misc-non-private-member-variables-in-classes)
};

class ForegroundHandler : public BasicHandler {
public:
    explicit ForegroundHandler(GameModel &model, ForegroundEntity &entity);

    void setBackground();
    void restoreBackground();
};

class MovableHandler : public ForegroundHandler {
public:
    explicit MovableHandler(GameModel &model, MovableEntity &entity);

    [[nodiscard]] std::vector<const Entity *> look(Direction direction);
    void move(Direction direction, int speed);

protected:
    template <typename T>
    std::vector<const Entity *> nearest(Direction direction, T cond) {
        int dist = INT_MAX;
        std::vector<const Entity *> res;
        for (const auto *entity : look(direction)) {
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

    void shoot();
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
