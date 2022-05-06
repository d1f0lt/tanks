#ifndef TANKS_HANDLER_H
#define TANKS_HANDLER_H

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
    GameModel &model;
    Entity &entity;
};

class ForegroundHandler : public BasicHandler {
public:
    explicit ForegroundHandler(GameModel &model_, ForegroundEntity &entity);

    void setBackground();
    void restoreBackground();
};

class MovableHandler : public ForegroundHandler {
public:
    explicit MovableHandler(GameModel &model_, MovableEntity &entity);

    [[nodiscard]] std::vector<const Entity *> look(Direction direction);
    void move(Direction direction, int speed);
};

class TankHandler : public MovableHandler {
public:
    explicit TankHandler(GameModel &model_, Tank &entity);

    void shoot();
};

class ProjectileHandler : public MovableHandler {
public:
    explicit ProjectileHandler(GameModel &model, MovableEntity &entity);

    void destroyByBullet(Entity &other);

    [[nodiscard]] bool isBreakOnNextTick();
    [[nodiscard]] bool isBreakOnCreation();
};

}  // namespace Tanks::model
#endif  // TANKS_HANDLER_H
