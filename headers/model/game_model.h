#ifndef TANKS_GAME_MODEL_H
#define TANKS_GAME_MODEL_H

#include <optional>
#include <queue>
#include <unordered_map>
#include "entity_holder.h"
#include "model/event.h"
#include "model/event_executor.h"
#include "model/game_map.h"
#include "model/grouped_entities.h"
#include "model/handler.h"
#include "model/playable-tank.h"

namespace Tanks::model {
class GameModel {
    friend EventExecutor;

    friend BasicHandler;
    friend ForegroundHandler;
    friend MovableHandler;
    friend TankHandler;
    friend ProjectileHandler;

public:
    explicit GameModel() = default;
    virtual ~GameModel() = default;

    [[nodiscard]] Entity &getByCoords(int col, int row);
    [[nodiscard]] std::optional<std::reference_wrapper<Entity>> getById(
        int entityId);

    void nextTick();

    void loadLevel(int level);

    [[nodiscard]] int getHeight() const;
    [[nodiscard]] int getWidth() const;

    [[nodiscard]] std::vector<const Entity *> getAll(EntityType type);
    [[nodiscard]] std::vector<std::vector<const Entity *>> getAll();

    [[nodiscard]] int getTick() const;

protected:
    void addEntity(std::unique_ptr<Entity> entity);
    void removeEntity(Entity &entity);

    virtual void executeAllEvents() = 0;
    void executeEvent(Event &event);

    void moveBullets();

    [[nodiscard]] BasicHandler &getHandler(Entity &entity);

    [[nodiscard]] const std::vector<std::vector<Entity *>> &getAllByLink();

    [[nodiscard]] int getIncrId();

    [[nodiscard]] GameMap &getMap();

private:
    GameMap map_;
    GroupedEntities groupedEntities_;
    std::unordered_map<int, Entity *> byId_;
    std::unordered_map<Entity *, BasicHandler *> handlers_;
    int currentTick_ = 0;
    int currentId_ = 0;
    EntityHolder entityHolder_;
};

}  // namespace Tanks::model

#endif  // TANKS_GAME_MODEL_H
