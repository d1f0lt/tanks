#ifndef TANKS_FOREGROUND_ENTITY_H
#define TANKS_FOREGROUND_ENTITY_H

#include "entity.h"
#include "game_map.h"

namespace Tanks::model {
class ForegroundEntity : public Entity {
public:
    ForegroundEntity(int left,
                     int top,
                     int width,
                     int height,
                     EntityType type,
                     GameMap &map);

protected:
    void restoreBackground();
    void setBackground();

    void move_(Direction dir);

protected:
    GameMap &map;
    std::vector<std::vector<Entity *>> background;
};

}  // namespace Tanks::model

#endif  // TANKS_FOREGROUND_ENTITY_H
