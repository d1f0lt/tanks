#ifndef TANKS_GAME_MODEL_H
#define TANKS_GAME_MODEL_H

#include <chrono>
#include <optional>
#include <queue>
#include <random>
#include <shared_mutex>
#include <unordered_map>
#include "entity_holder.h"
#include "model/event.h"
#include "model/event_executor.h"
#include "model/game_map.h"
#include "model/grouped_entities.h"
#include "model/handler.h"
#include "model/tank_handler.h"

namespace Tanks::model {
class Spawner;  // Todo spawner fwd
struct PlayerSkillsTmp {
    const int tankSpeed = DEFAULT_TANK_SPEED;
    const int bulletSpeed = DEFAULT_BULLET_SPEED;
    const int reloadTicks = DEFAULT_RELOAD_TICKS;
};

using PlayerSkills = PlayerSkillsTmp;

class GameModel {
    friend EventExecutor;

    friend BasicHandler;
    friend ForegroundHandler;
    friend MovableHandler;
    friend TankHandler;
    friend ProjectileHandler;
    friend BonusHandler;

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

    [[nodiscard]] std::vector<const Entity *> getAll(EntityType type) const;
    [[nodiscard]] std::vector<std::vector<const Entity *>> getAll() const;

    [[nodiscard]] int getTick() const;

protected:
    void addEntity(std::unique_ptr<Entity> entity);
    void eraseEntity(Entity &entity);

    virtual void executeAllEvents() = 0;
    void executeEvent(Event &event);

    void moveBullets();

    [[nodiscard]] BasicHandler &getHandler(Entity &entity);

    [[nodiscard]] const std::vector<std::vector<Entity *>> &getAllByLink();

    [[nodiscard]] GameMap &getMap();

    [[nodiscard]] int getIncrId();
    [[nodiscard]] int getRnd();

    [[nodiscard]] std::shared_mutex &getMutex() const;

    // TODO
    //  [[nodiscard]] PlayerSkillsTmp getPlayerSkills(int id);

private:
    GameMap map_;
    GroupedEntities groupedEntities_;
    std::unordered_map<int, Entity *> byId_;
    std::unordered_map<Entity *, BasicHandler *> handlers_;
    int currentTick_ = 0;
    int currentId_ = 0;
    std::unordered_map<int, PlayerSkills> players_;
    std::mt19937 rnd{42};
    EntityHolder entityHolder_;
    mutable std::shared_mutex sharedMutex_;
};

}  // namespace Tanks::model

#endif  // TANKS_GAME_MODEL_H
