#ifndef TANKS_MOVABLE_ENTITY_H
#define TANKS_MOVABLE_ENTITY_H

#include "model/foreground_entity.h"

namespace Tanks::model {
class GameModel;  // TODO forward declaration

class MovableEntity : public ForegroundEntity {
public:
    friend GameModel;

    explicit MovableEntity(int col,
                           int row,
                           int width,
                           int height,
                           EntityType type,
                           Direction direction,
                           GameMap &map);

    [[nodiscard]] Direction getDirection() const;

protected:
    void setDirection(Direction dir);

    void move(Direction dir);

private:
    Direction direction;
};
}  // namespace Tanks::model

#endif  // TANKS_MOVABLE_ENTITY_H
