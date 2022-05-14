#ifndef TANKS_MOVABLE_ENTITY_H
#define TANKS_MOVABLE_ENTITY_H

#include "model/foreground_entity.h"

namespace Tanks::model {

class MovableEntity : public ForegroundEntity {
public:
    friend MovableHandler;

    explicit MovableEntity(int left,
                           int top,
                           int entityId,
                           std::unique_ptr<MovableHandler> handler,
                           Direction direction);

    [[nodiscard]] Direction getDirection() const;
    [[nodiscard]] virtual int getSpeed() const = 0;

protected:
    void setDirection(Direction direction);
    void move(Direction direction, int speed);

private:
    Direction direction_;
};
}  // namespace Tanks::model

#endif  // TANKS_MOVABLE_ENTITY_H
