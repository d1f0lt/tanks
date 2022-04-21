#ifndef TANKS_GAME_MODEL_H
#define TANKS_GAME_MODEL_H

#include "entity.h"
#include "entity_holder.h"
#include "game_map.h"
#include "grouped_entities.h"
#include "tank.h"

namespace Tanks::model {
class GameModel {
public:
    explicit GameModel() = default;
    //    explicit GameModel(const std::string &filename);

    [[nodiscard]] Entity &getEntityByCoords(int col, int row);

    void nextTick();

    [[nodiscard]] PlayableTank &spawnPlayableTank(int left, int top);

    void loadLevel(int level);

private:
    Entity &addEntity(std::unique_ptr<Entity> entity);
    void removeEntity(Entity &entity);

    GameMap map;
    GroupedEntities groupedEntities;
    EntityHolder entityHolder;
};
}  // namespace Tanks::model

#endif  // TANKS_GAME_MODEL_H
