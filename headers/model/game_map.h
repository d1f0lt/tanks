#ifndef TANKS_GAME_MAP_H
#define TANKS_GAME_MAP_H

#include <vector>
#include "constants.h"
#include "entity.h"

namespace Tanks::model {

class GameMap {
public:
    explicit GameMap(size_t width = MAP_WIDTH, size_t height = MAP_HEIGHT);

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;

    [[nodiscard]] Entity &getEntityByCoords(int col, int row) const;

    void insert(Entity &entity);
    void erase(Entity &entity);
    void eraseByCoords(int col, int row);

private:
    std::vector<std::vector<Entity *>> map;
};
}  // namespace Tanks::model

#endif  // TANKS_GAME_MAP_H
