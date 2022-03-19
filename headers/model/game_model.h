#ifndef TANKS_GAME_MODEL_H
#define TANKS_GAME_MODEL_H

#include <queue>
#include "entity_holder.h"
#include "model/event.h"
#include "model/game_map.h"
#include "model/grouped_entities.h"
#include "model/tank.h"

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
    std::queue<std::unique_ptr<Event>> que{};  // TODO threadsafe
};
}  // namespace Tanks::model

#endif  // TANKS_GAME_MODEL_H
