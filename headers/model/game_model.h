#ifndef TANKS_GAME_MODEL_H
#define TANKS_GAME_MODEL_H

#include <queue>
#include <unordered_map>
#include "entity_holder.h"
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
    friend ProjectileHandler;

public:
    explicit GameModel() = default;

    [[nodiscard]] Entity &getByCoords(int col, int row);
    [[nodiscard]] Entity &getById(int id);

    void nextTick();

    [[nodiscard]] PlayableTank &spawnPlayableTank(int left, int top);

    void loadLevel(int level);

    [[nodiscard]] int getHeight() const;
    [[nodiscard]] int getWidth() const;

private:
    Entity &addEntity(std::unique_ptr<Entity> entity);
    void removeEntity(Entity &entity);

    GameMap map;
    GroupedEntities groupedEntities;
    EntityHolder entityHolder;
    std::unordered_map<int, Entity *> byid;
    std::unordered_map<Entity *, BasicHandler *> handlers;
};
}  // namespace Tanks::model

#endif  // TANKS_GAME_MODEL_H
