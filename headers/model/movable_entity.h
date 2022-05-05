#ifndef TANKS_MOVABLE_ENTITY_H
#define TANKS_MOVABLE_ENTITY_H

#include "model/foreground_entity.h"

namespace Tanks::model {

class MovableEntity : public ForegroundEntity {
public:
    friend MovableHandler;

    explicit MovableEntity(int left,
                           int right,
                           std::unique_ptr<BasicHandler> handler_,
                           Direction direction,
                           int speed);

    explicit MovableEntity(int left,
                           int top,
                           int id,
                           std::unique_ptr<BasicHandler> handler,
                           Direction direction,
                           int speed);

    [[nodiscard]] Direction getDirection() const;

    [[nodiscard]] int getSpeed() const;

protected:
    void setDirection(Direction dir);

    void move(Direction dir, int speed_);

private:
    Direction direction;
    int speed = 0;
};
}  // namespace Tanks::model

#endif  // TANKS_MOVABLE_ENTITY_H
