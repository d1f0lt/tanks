#ifndef TANKS_GAME_MODEL_H
#define TANKS_GAME_MODEL_H

#include <queue>
#include "entity_holder.h"
#include "model/event.h"
#include "model/game_map.h"
#include "model/grouped_entities.h"
#include "model/handler.h"
#include "model/playable-tank.h"

namespace Tanks::model {
class GameModel {
    friend BasicHandler;
    friend ForegroundHandler;
    friend MovableHandler;
    friend TankHandler;

public:
    explicit GameModel() = default;

    [[nodiscard]] Entity &getEntityByCoords(int col, int row);

    void nextTick();

    [[nodiscard]] PlayableTank &spawnPlayableTank(int left, int top);

    void loadLevel(int level);

private:
    Entity &addEntity(std::unique_ptr<Entity> entity);
    Entity &addEntity(std::unique_ptr<Entity> entity, BasicHandler *handler);
    void removeEntity(Entity &entity);

    GameMap map;
    GroupedEntities groupedEntities;
    EntityHolder entityHolder;
    std::unordered_map<Entity *, BasicHandler *> handlers;
    std::queue<std::unique_ptr<Event>> que{};  // TODO threadsafe
};
}  // namespace Tanks::model

#endif  // TANKS_GAME_MODEL_H
