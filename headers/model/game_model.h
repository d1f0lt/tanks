#ifndef TANKS_GAME_MODEL_H
#define TANKS_GAME_MODEL_H

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <unordered_map>
#include "model/entity_holder.h"
#include "model/event.h"
#include "model/event_executor.h"
#include "model/game_map.h"
#include "model/grouped_entities.h"
#include "model/handler.h"
#include "model/tank_handler.h"

namespace Tanks::model {
class Spawner;  // Todo spawner fwd

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
    GameModel(GameModel &&) = delete;
    GameModel(const GameModel &) = delete;
    GameModel &operator=(const GameModel &) = delete;
    GameModel &operator=(GameModel &&) = delete;

    virtual void finishGame() noexcept = 0;

    [[nodiscard]] Entity &getByCoords(int col, int row);
    [[nodiscard]] std::optional<std::reference_wrapper<Entity>> getById(
        int entityId);

    virtual void nextTick() = 0;

    void loadLevel(int level);
    void loadLevel(const std::string &filename);

    [[nodiscard]] int getHeight() const;
    [[nodiscard]] int getWidth() const;

    [[nodiscard]] std::vector<const Entity *> getAll(EntityType type) const;
    [[nodiscard]] std::vector<std::vector<const Entity *>> getAll() const;

    [[nodiscard]] int getTick() const;

    [[nodiscard]] bool wasShootThisTurn() const;
    [[nodiscard]] bool wasDestroyedBlockThisTurn() const;

protected:
    void addEntity(std::unique_ptr<Entity> entity);
    virtual void eraseEntity(Entity &entity);

    virtual void executeAllEvents() = 0;
    virtual bool executeEvent(Event &event);
    void moveBullets();

    [[nodiscard]] BasicHandler &getHandler(Entity &entity);

    [[nodiscard]] IncrId getIncrId();
    [[nodiscard]] int getRnd();

    [[nodiscard]] GameMap &getMap();
    [[nodiscard]] const std::vector<std::vector<Entity *>> &getAllByLink();

    [[nodiscard]] std::mutex &getMutex() const;
    [[nodiscard]] std::condition_variable &getCondvar();

    [[nodiscard]] bool getIsFinished() const;

    void setFinished();
    void incrTick(int add = 1);

    void setWasShootThisTurn(bool wasShootThisTurn);
    void setWasDestroyedBlockThisTurn(bool wasDestroyedBlockThisTurn);

private:
    GameMap map_;
    GroupedEntities groupedEntities_;
    std::unordered_map<Entity *, BasicHandler *> handlers_;
    std::unordered_map<int, Entity *> byId_;
    EntityHolder entityHolder_;  // must be after handlers_

    std::atomic<int> currentTick_ = 0;
    IncrId currentId_{0};
    std::mt19937 rnd{42};

    bool wasShootThisTurn_ = false;
    bool wasDestroyedBlockThisTurn_ = false;
    std::atomic<bool> isFinished_ = false;

    mutable std::mutex modelMutex_;
    std::condition_variable condvar_;
};

}  // namespace Tanks::model

#endif  // TANKS_GAME_MODEL_H
