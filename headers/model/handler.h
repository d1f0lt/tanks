#ifndef TANKS_HANDLER_H
#define TANKS_HANDLER_H

#include "entity.h"

namespace Tanks::model {
class Entity;
class MovableEntity;
class ForegroundEntity;
class Tank;

class GameModel;
class BasicHandler {
public:
    explicit BasicHandler(GameModel &model_);

    BasicHandler(const BasicHandler &) = delete;
    BasicHandler(BasicHandler &&) = delete;
    BasicHandler &operator=(const BasicHandler &) = delete;
    BasicHandler &operator=(BasicHandler &&) = delete;

    virtual ~BasicHandler() = default;

    [[nodiscard]] std::vector<const Entity *> look(const Entity &entity,
                                                   Direction direction);

    virtual void move(MovableEntity &entity, Direction direction);

    virtual void setBackground(ForegroundEntity &entity) = 0;
    virtual void restoreBackground(ForegroundEntity &entity) = 0;

    virtual void shoot(Tank &entity);

protected:
    GameModel &model;
};

class ForegroundHandler : public BasicHandler {
public:
    explicit ForegroundHandler(GameModel &model_);

    void setBackground(ForegroundEntity &entity) final;
    void restoreBackground(ForegroundEntity &entity) final;
};

class MovableHandler : public ForegroundHandler {
public:
    explicit MovableHandler(GameModel &model_);

    void move(MovableEntity &entity, Direction direction) final;
};

class TankHandler : public MovableHandler {
public:
    explicit TankHandler(GameModel &model_);

    void shoot(Tank &entity) final;
};

}  // namespace Tanks::model
#endif  // TANKS_HANDLER_H
