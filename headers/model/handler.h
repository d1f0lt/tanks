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

    virtual void setBackground() = 0;
    virtual void restoreBackground() = 0;

    [[nodiscard]] virtual std::vector<const Entity *> look(Direction direction);

    virtual void move(Direction direction, int speed);

    virtual void shoot();

protected:
    GameModel &model;
    Entity &entity;
};

class ForegroundHandler : public BasicHandler {
public:
    explicit ForegroundHandler(GameModel &model_, ForegroundEntity &entity);

    void setBackground() final;
    void restoreBackground() final;
};

class MovableHandler : public ForegroundHandler {
public:
    explicit MovableHandler(GameModel &model_, MovableEntity &entity);

    [[nodiscard]] std::vector<const Entity *> look(Direction direction) final;
    void move(Direction direction, int speed) final;
};

class TankHandler : public MovableHandler {
public:
    explicit TankHandler(GameModel &model_, Tank &entity);

    void shoot() final;
};

}  // namespace Tanks::model
#endif  // TANKS_HANDLER_H
